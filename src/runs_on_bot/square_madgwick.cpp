#include <Arduino.h>

// #define MURMECHA_REV1_MOTOR_UPGRADE

#include <robot.hpp>
#include <rgb.hpp>
#include <uv.hpp>
#include <display.hpp>
#include <motors.hpp>
#include <pins.hpp>
#include <WiFi.h>
#include <driver/mcpwm.h>
#include "imu_madgwick_integration.h"
#include "imu_integral_integration.h"
#include <tuple>

#include <complex>

#define bufferSize 256

using namespace murmecha;

float vel = 60.0f;
// für bot 16 und 22
float R_perp = 68.0f/2.0f;

murmecha::math::Vector3 computed_drifts;
const int sample_count = 1000;
murmecha::math::Vector3 samples[sample_count];

// PID Params
float p = 60;
float i = 0.025;
float d = 9;
 
const float sensitivity = 1.0f;

auto q_initial_pid = Quaternion::identity();
 
float previous_error = 0;
float integral = 0;

//Wifi Setup
extern WiFiUDP udp;

// misst Drift des Gyroskops
murmecha::math::Vector3 measure_gyro_drift() {

  for (uint32_t i = 0; i < sample_count; ++i) {
    murmecha::imu::read_accel_and_gyro();
    samples[i] = murmecha::imu::get_gyroscope();
    delay(1);
  }

  murmecha::math::Vector3 mean;
  
  for (uint32_t i = 0; i < sample_count; ++i) {
    mean = mean + samples[i];
    Serial.printf("%f %f %f\n", samples[i].x, samples[i].y, samples[i].z);
  }

  return {mean.x / sample_count, mean.y / sample_count, mean.z / sample_count};
  
}

// Gerade fahren mit motorbasierter Fahrweise
void drive_segment_motors(float length, float vm) {
  float time = length / vm;
  motors::set_linear_velocities(vm, vm);
  delay(time*1000);
  motors::set_linear_velocities(0, 0);
}

// Kreis fahren mit motorbasierter Fahrweise
void drive_circle_motors(float radius, float vm) {
  float R_l = radius - R_perp;
  float R_r = radius + R_perp;

  float v_r = 2 * vm / (1 + (R_l / R_r));
  float v_l = v_r * (R_l / R_r);
  murmecha::motors::set_linear_velocities(v_l, v_r);
}

// Kurve fahren mit motorbasierter Fahrweise
void drive_curve_motors(float radius, float angle, float vm) {
  int start_curve = 1;
  int end_curve = 0;

  float dist = radius * angle;
  float time = dist / vm;

  // vor der Kurve senden an Laptop im Format int
  // udp.beginPacket(IPAddress(10, 0, 2, 137), 4444);
  // udp.write((uint8_t *) &start_curve, sizeof(start_curve));
  // udp.endPacket();

  drive_circle_motors(radius, vm);

  delay(time * 1000);

  // nach der Kurve senden an Laptop im Format int
  // udp.beginPacket(IPAddress(10, 0, 2, 137), 4444);
  // udp.write((uint8_t *) &end_curve, sizeof(end_curve));
  // udp.endPacket();

  motors::set_linear_velocities(0, 0);

}

// Kurve fahren mit numerischer Integration
void drive_curve_integral(murmecha::math::Vector3 computed_drifts,float radius, float angle, float vm) {
  int start_curve = 1;
  int end_curve = 0;

  reset_orientation_integral();
  murmecha::math::Vector3 o_initial = {0, 0, 0}; 
  float current_angle = 0; 

  // R_l mit + --> rechtskurve, R_r mit - --> linkskurve
  // R_l mit - --> rechtskurve, R_r mit + --> linkskurve
  float R_l = radius - R_perp;
  float R_r = radius + R_perp;

  float v_r = 2 * vm / (1 + (R_l / R_r));
  float v_l = v_r * (R_l / R_r);

  // wenn Quadrat mit scharfen Ecken (radius ~ 0) gefahren werden soll
  if(abs(radius)<1e-5){
    v_l = -vm;
    v_r = vm;
  }

  // vor der Kurve senden an Laptop im Format int-f-fff
  udp.beginPacket(IPAddress(10, 0, 2, 137), 4444);
  udp.write((uint8_t *) &start_curve, sizeof(start_curve));
  udp.write((uint8_t *) &current_angle, sizeof(current_angle));
  udp.write((uint8_t *) &orientation_integral, sizeof(orientation_integral));
  udp.endPacket();

  int while_counter = 0;
  uint64_t start_time = micros();
  while(current_angle < angle) {
    while_counter++;

    // Abbruchkriterium, falls einer der Driftwerte > pi
    if (computed_drifts.x > M_PI || computed_drifts.y > M_PI || computed_drifts.z > M_PI) {
      Serial.printf("Drift ist zu hoch: (%f %f %f)\n", computed_drifts.x, computed_drifts.y, computed_drifts.z);
      break;
    }

    // Einsatz numerische Integration
    update_orientation_integral(computed_drifts);

    motors::set_linear_velocities(v_l, v_r);
    current_angle = orientation_integral.z - o_initial.z;
    delay(1);
  }
  uint64_t end_time = micros();
  auto time_diff = end_time - start_time;

  // nach der Kurve senden an Laptop im Format int-f-int-fff-long
  udp.beginPacket(IPAddress(10, 0, 2, 137), 4444);
  udp.write((uint8_t *) &end_curve, sizeof(end_curve));
  udp.write((uint8_t *) &current_angle, sizeof(current_angle));
  udp.write((uint8_t *) &while_counter, sizeof(while_counter));
  udp.write((uint8_t *) &orientation_integral, sizeof(orientation_integral));
  udp.write((uint8_t *) &time_diff, sizeof(time_diff));
  udp.endPacket();

  motors::set_linear_velocities(0, 0);
}

// Kurve fahren mit Madgwick-Filter
void drive_curve_madgwick(murmecha::math::Vector3 computed_drifts, float radius, float target_angle, float vm) {
  int start_curve = 1;
  int end_curve = 0;

  reset_orientation_madgwick();
  Quaternion q_initial = Quaternion::identity();
  float current_angle = 0; 

  // R_l mit + --> Rechtskurve, R_r mit - --> Linkskurve
  // R_l mit - --> Rechtskurve, R_r mit + --> Linkskurve
  float R_l = radius - R_perp;
  float R_r = radius + R_perp;

  float v_r = 2 * vm / (1 + (R_l / R_r));
  float v_l = v_r * (R_l / R_r);

  // wenn Quadrat mit scharfen Ecken (radius ~ 0) gefahren werden soll
  if(abs(radius)<1e-5){
    v_l = -vm;
    v_r = vm;
  }

  // vor der Kurve senden an Laptop im Format int-f-ffff
  // udp.beginPacket(IPAddress(10, 0, 2, 137), 4444);
  // udp.write((uint8_t *) &start_curve, sizeof(start_curve));
  // udp.write((uint8_t *) &current_angle, sizeof(current_angle));
  // udp.write((uint8_t *) &orientation_madgwick, sizeof(orientation_madgwick));
  // udp.endPacket();

  int while_counter = 0;
  uint64_t start_time = micros();
  while(current_angle < target_angle) {
    while_counter++;

    // Abbruchkriterium, falls einer der Driftwerte > pi
    if (abs(computed_drifts.x) > M_PI || abs(computed_drifts.y) > M_PI || abs(computed_drifts.z) > M_PI) {
      Serial.printf("Drift ist zu hoch: (%f %f %f)\n", computed_drifts.x, computed_drifts.y, computed_drifts.z);
      break;
    }

    // Einsatz des Magdwick-Filters
    update_orientation_madgwick(computed_drifts);

    motors::set_linear_velocities(v_l, v_r);
    current_angle = calculateRotationAngle(q_initial, orientation_madgwick);
    delay(1);
  }
  uint64_t end_time = micros();
  auto time_diff = end_time - start_time;

  // nach der Kurve senden an Laptop im Format int-f-ffff-int-long
  // udp.beginPacket(IPAddress(10, 0, 2, 137), 4444);
  // udp.write((uint8_t *) &end_curve, sizeof(end_curve));
  // udp.write((uint8_t *) &current_angle, sizeof(current_angle));
  // udp.write((uint8_t *) &orientation_madgwick, sizeof(orientation_madgwick));
  // udp.write((uint8_t *) &while_counter, sizeof(while_counter));
  // udp.write((uint8_t *) &time_diff, sizeof(time_diff));
  // udp.endPacket();

  motors::set_linear_velocities(0, 0);
}

// PID-Regler, gibt nötige änderung der motorgeschwindigkeit als float zurück
float pid_control(float target_angle, float current_angle) {

  std::complex<float> target_dir(cosf(target_angle), sinf(target_angle));
  std::complex<float> current_dir(cosf(current_angle), sinf(current_angle));

  auto dir_diff = conj(target_dir) * current_dir;
  float error = atan2f(dir_diff.imag(), dir_diff.real());

  integral += error;
  float derivative = error - previous_error;
  previous_error = error;

  return p * error + i * integral + d * derivative;
}

// PID-Regler zurücksetzen
void reset_pid() {
  previous_error = 0;
  integral = 0;
}

// PID-Regler, Output umgewandelt in Radgeschwindigkeiten 
std::tuple<float, float> pid_to_wheel_speeds(float pid_output, float base_speed) {
    return {
      base_speed + sensitivity * pid_output,
      base_speed - sensitivity * pid_output
    };
}


void setup_square() {
  Serial.begin(115200);
  murmecha::config_t config;
  config.use_microstepping = false;
  murmecha::begin(config);
  // murmecha::uv::set(1.0f);
  // murmecha::rgb::set_led_color(0, 64, 0, 0);
  murmecha::display::invert();
  murmecha::display::clear();
  murmecha::display::draw_tracking_code(16);
  murmecha::display::draw_info_screen(16);
  murmecha::display::update();
  murmecha::imu::set_accel_data_rate(murmecha::imu::DATA_RATE_1_6kHz);
  murmecha::imu::set_gyro_data_rate(murmecha::imu::DATA_RATE_1_6kHz);

  // IP-Adresse (6 für bot mit nummer 6), Gateway, an wen senden (zuhause)
  WiFi.config(IPAddress(10, 0, 1, 22), IPAddress(10, 0, 2, 1), IPAddress(255, 255, 0, 0));
  //WLAN, Passwort (UNI)
  WiFi.begin("DeziWLAN", "Ingwer-Zitrone");
  while (!WiFi.isConnected()){
    delay(100);
    Serial.println("Connecting to WiFi..");
  }

  delay(2000);
  murmecha::motors::beep(1200,500);

  computed_drifts = measure_gyro_drift();

  //Serial.printf("Drift: (%f %f %f)\n", computed_drifts.x, computed_drifts.y, computed_drifts.z);
}


void loop_square() {

  // Gerade fahren mit motorbasierter Fahrweise
  drive_segment_motors(250.0f, vel);

  // runde Ecke fahren mit motorbasierter Fahrweise
  drive_curve_motors(100, M_PI/2, vel);


  // runde Ecke fahren mit numerischer Integration
  // drive_curve_integral(computed_drifts, 100, M_PI/2, vel);

  // Ecke fahren mit numerischer Integration
  // drive_curve_integral(computed_drifts, 0, M_PI/2, vel);


  // Ecke fahren mit Madgwick-Filter
  // drive_curve_madgwick(computed_drifts, 0, M_PI/2, vel);

  // runde Ecke fahren mit Madgwick-Filter
  // drive_curve_madgwick(computed_drifts, 100, M_PI/2, vel);


  murmecha::display::clear();
  murmecha::display::draw_info_screen(16);
  murmecha::display::draw_tracking_code(16);
  murmecha::display::update();
}