#include <Arduino.h>
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

#define bufferSize 256

using namespace murmecha;

float vel = 60.0f;
// für bot 16 und 22
float R_perp = 68.0f/2.0f;

murmecha::math::Vector3 computed_drifts;
const int sample_count = 1000;
murmecha::math::Vector3 samples[sample_count];

// pid-params
float p = 0.5;
float i = 0.01;
float d = 0.1;
auto q_initial_pid = Quaternion::identity();

float previous_error = 0;
float integral = 0;

//Wifi Setup
extern WiFiUDP udp;

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

void drive_circle_motors(float radius, float vm) {
  float R_l = radius - R_perp;
  float R_r = radius + R_perp;

  float v_r = 2 * vm / (1 + (R_l / R_r));
  float v_l = v_r * (R_l / R_r);
  murmecha::motors::set_linear_velocities(v_l, v_r);
}

void drive_segment_motors(float length, float vm) {
  float time = length / vm;
  motors::set_linear_velocities(vm, vm);
  delay(time*1000);
  motors::set_linear_velocities(0, 0);
}

// pid-controller für winkel-regelung, gibt nötige änderung der motorgeschwindigkeit als float zurück
float pid_control(float target_angle, float current_angle) {
  float error = target_angle - current_angle;

  // fehler zwischen pi/-pi bringen, modulo 2pi, dann verschiebung um -pi
  error = fmod(error + M_PI, 2 * M_PI) - M_PI;

  integral += error;
  float derivative = error - previous_error;
  previous_error = error;

  return p * error + i * integral + d * derivative;
}

// setzt alle params für pid-steuerung zurück
void reset_pid() {
  previous_error = 0;
  integral = 0;
}

// pid-output umgewandelt in radgeschwindigkeiten und gibt tupel aus
std::tuple<float, float> pid_to_wheel_speeds(float pid_output, float base_speed) {
    return {
      base_speed - pid_output,
      base_speed + pid_output
    };
}

// fährt strecke mit pid-regler
void drive_segment_pid(murmecha::math::Vector3 computed_drifts, float length, float vm, float final_angle) {
  float traveled_distance = 0;
  float dt = 0.01;

  while (traveled_distance < length) {
      update_orientation_madgwick(computed_drifts);
      float current_angle = calculateRotationAngle(q_initial_pid, orientation_madgwick);
      float pid_output = pid_control(final_angle, current_angle);

      auto [v_l, v_r] = pid_to_wheel_speeds(pid_output, vm);

      motors::set_linear_velocities(v_l, v_r);
      delay(dt * 1000);

      // wie errechne ich distanz? kann so nicht bleiben
      traveled_distance += vm * dt;
  }

  motors::set_linear_velocities(0, 0);
}

// fährt kurve mit pid-regler
void drive_curve_pid(murmecha::math::Vector3 computed_drifts, float radius, float vm_cm_per_min, float final_angle) {
    float start_angle = final_angle - M_PI / 2;
    float length = radius * (M_PI / 2);  
    float traveled_distance = 0;
    float dt = 0.01;

    float vm = vm_cm_per_min / 6000.0; 

    while (traveled_distance < length) {
        update_orientation_madgwick(computed_drifts);

        float progress = traveled_distance / length;
        float target_angle = start_angle + progress * (final_angle - start_angle);

        float pid_output = pid_control(target_angle, current_angle);

        auto [v_l, v_r] = pid_to_wheel_speeds(pid_output, vm);  

        motors::set_linear_velocities(v_l, v_r);
        delay(dt * 1000);

        traveled_distance += vm * dt;
    }

    motors::set_linear_velocities(0, 0);  
}

// strecke fahren mit madgwick, muss noch überarbeitet werden
void drive_segment_madgwick(float length, float vm) {
  int start_segment = 2;
  int end_segment = 3;

  reset_orientation_madgwick();
  Quaternion q_initial = Quaternion::identity();
  float current_length = 0; 
  float current_angle = 0;

  float v_r = vm;
  float v_l = vm;

  // vor dem Segment senden an david int-f-f-ffff
  udp.beginPacket(IPAddress(10, 0, 2, 137), 4444);
  udp.write((uint8_t *) &start_segment, sizeof(start_segment));
  udp.write((uint8_t *) &current_angle, sizeof(current_angle));
  udp.write((uint8_t *) &current_length, sizeof(current_length));
  udp.write((uint8_t *) &orientation_madgwick, sizeof(orientation_madgwick));
  udp.endPacket();

  int while_counter = 0;
  uint64_t start_time = micros();
  while(current_length < length) {
    while_counter++;

    update_orientation_madgwick(computed_drifts);
    motors::set_linear_velocities(v_l, v_r);

    // hier bin ich nicht sicher: zwischen welchen punkten soll ich die distanz berechnen?
    current_length = calculateTranslationDistance(q_initial, orientation_madgwick);
    delay(1);
  }
  uint64_t end_time = micros();
  auto time_diff = end_time - start_time;

  // nach dem Segment senden an david int-f-ffff-int-long
  udp.beginPacket(IPAddress(10, 0, 2, 137), 4444);
  udp.write((uint8_t *) &end_segment, sizeof(end_segment));
  udp.write((uint8_t *) &current_length, sizeof(current_length));
  udp.write((uint8_t *) &orientation_madgwick, sizeof(orientation_madgwick));
  udp.write((uint8_t *) &time_diff, sizeof(time_diff));
  udp.endPacket();

  motors::set_linear_velocities(0, 0);
}

// kurve fahren mit madgwick
void drive_curve_madgwick(murmecha::math::Vector3 computed_drifts, float radius, float target_angle, float vm) {
  int start_curve = 1;
  int end_curve = 0;

  reset_orientation_madgwick();
  Quaternion q_initial = Quaternion::identity();
  float current_angle = 0; 

  // R_l mit + --> rechtskurve, R_r mit - --> linkskurve
  // R_l mit - --> rechtskurve, R_r mit + --> linkskurve
  float R_l = radius - R_perp;
  float R_r = radius + R_perp;

  float v_r = 2 * vm / (1 + (R_l / R_r));
  float v_l = v_r * (R_l / R_r);

  // wenn square mit scharfen ecken
  if(abs(radius)<1e-5){
    v_l = -vm;
    v_r = vm;
  }

  // vor der Kurve senden an david int-f-ffff
  udp.beginPacket(IPAddress(10, 0, 2, 137), 4444);
  udp.write((uint8_t *) &start_curve, sizeof(start_curve));
  udp.write((uint8_t *) &current_angle, sizeof(current_angle));
  udp.write((uint8_t *) &orientation_madgwick, sizeof(orientation_madgwick));
  udp.endPacket();

  int while_counter = 0;
  uint64_t start_time = micros();
  while(current_angle < target_angle) {
    while_counter++;

    // Abbruchkriterium, falls einer der Driftwerte > pi
    if (abs(computed_drifts.x) > M_PI || abs(computed_drifts.y) > M_PI || abs(computed_drifts.z) > M_PI) {
      Serial.printf("Drift ist zu hoch: (%f %f %f)\n", computed_drifts.x, computed_drifts.y, computed_drifts.z);
      break;
    }

    update_orientation_madgwick(computed_drifts);
    motors::set_linear_velocities(v_l, v_r);
    current_angle = calculateRotationAngle(q_initial, orientation_madgwick);
    delay(1);
  }
  uint64_t end_time = micros();
  auto time_diff = end_time - start_time;

  // nach der Kurve senden an david int-f-ffff-int-long
  udp.beginPacket(IPAddress(10, 0, 2, 137), 4444);
  udp.write((uint8_t *) &end_curve, sizeof(end_curve));
  udp.write((uint8_t *) &current_angle, sizeof(current_angle));
  udp.write((uint8_t *) &orientation_madgwick, sizeof(orientation_madgwick));
  udp.write((uint8_t *) &while_counter, sizeof(while_counter));
  udp.write((uint8_t *) &time_diff, sizeof(time_diff));
  udp.endPacket();

  motors::set_linear_velocities(0, 0);
}

// kurve fahren mit integral
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

  if(abs(radius)<1e-5){
    v_l = -vm;
    v_r = vm;
  }

  // vor der Kurve senden an david int-f-ffff
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
    update_orientation_integral(computed_drifts);
    motors::set_linear_velocities(v_l, v_r);
    current_angle = orientation_integral.z - o_initial.z;
    delay(1);
  }
  uint64_t end_time = micros();
  auto time_diff = end_time - start_time;

  // nach der Kurve senden an david f-ffff-int-long-int
  udp.beginPacket(IPAddress(10, 0, 2, 137), 4444);
  udp.write((uint8_t *) &end_curve, sizeof(end_curve));
  udp.write((uint8_t *) &current_angle, sizeof(current_angle));
  udp.write((uint8_t *) &while_counter, sizeof(while_counter));
  udp.write((uint8_t *) &orientation_integral, sizeof(orientation_integral));
  udp.write((uint8_t *) &time_diff, sizeof(time_diff));
  udp.endPacket();

  motors::set_linear_velocities(0, 0);
}

void custom_setup_square_madgwick() {
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

  // WiFi.begin("Vodafone-2584", "4EgXXA7R93x9ypCz");
  // while (!WiFi.isConnected()){
  //   delay(100);
  //   Serial.println("Connecting to WiFi..");
  // }

  // //4444 port auf meinem Laptop
  // udp.begin(4444);

  delay(2000);
  murmecha::motors::beep(1200,500);

  computed_drifts = measure_gyro_drift();

  Serial.printf("Drift: (%f %f %f)\n", computed_drifts.x, computed_drifts.y, computed_drifts.z);
}

void custom_loop_square_madgwick() {

  drive_segment_motors(250.0f, vel);

  // Ecke fahren
  // drive_curve_madgwick(computed_drifts, 0, M_PI/2, vel);

  // runde Ecke fahren
  drive_curve_madgwick(computed_drifts, 100, M_PI/2, vel);

  // runde Ecke fahren mit integral
  // drive_curve_integral(computed_drifts, 100, M_PI/2, vel);

  // Ecke fahren
  // drive_curve_integral(computed_drifts, 0, M_PI/2, vel);

  murmecha::display::clear();
  murmecha::display::draw_info_screen(16);
  murmecha::display::draw_tracking_code(16);
  murmecha::display::update();
}

void custom_setup_square_pid() {
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

  Serial.printf("Drift: (%f %f %f)\n", computed_drifts.x, computed_drifts.y, computed_drifts.z);
}

void custom_loop_square_pid() {

  // kalibrierung
  // madgwick aktualisieren
  // pid aktualisieren
  // wheel speeds aktualisieren
  
  murmecha::display::clear();
  murmecha::display::draw_info_screen(16);
  murmecha::display::draw_tracking_code(16);
  murmecha::display::update();

}