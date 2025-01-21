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

#define bufferSize 256

using namespace murmecha;

float vel = 60.0f;
// für bot 16 und 22
float R_perp = 68.0f/2.0f;

murmecha::math::Vector3 computed_drifts;
const int sample_count = 1000;
murmecha::math::Vector3 samples[sample_count];

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
  }

  return {mean.x / sample_count, mean.y / sample_count, mean.z / sample_count};
  
}

void drive_circle(float radius, float vm) {
  float R_l = radius - R_perp;
  float R_r = radius + R_perp;

  float v_r = 2 * vm / (1 + (R_l / R_r));
  float v_l = v_r * (R_l / R_r);
  murmecha::motors::set_linear_velocities(v_l, v_r);
}

void drive_segment(float length, float vm) {
  float time = length / vm;
  motors::set_linear_velocities(vm, vm);
  delay(time*1000);
  motors::set_linear_velocities(0, 0);
}

// kurve fahren mit madgwick
void drive_curve_madgwick(murmecha::math::Vector3 computed_drifts,float radius, float angle, float vm) {
  int start_curve = 1;
  int end_curve = 0;

  reset_orientation();
  Quaternion q_initial = Quaternion::identity();
  float current_angle = 0; 

  // R_l mit + --> rechtskurve, R_r mit - --> linkskurve
  // R_l mit - --> rechtskurve, R_r mit + --> linkskurve
  float R_l = radius - R_perp;
  float R_r = radius + R_perp;

  float v_r = 2 * vm / (1 + (R_l / R_r));
  float v_l = v_r * (R_l / R_r);

  // vor der Kurve senden an david f-ffff-int
  udp.beginPacket(IPAddress(10, 0, 2, 137), 4444);
  udp.write((uint8_t *) &current_angle, sizeof(current_angle));
  udp.write((uint8_t *) &orientation, sizeof(orientation));
  udp.write((uint8_t *) &start_curve, sizeof(start_curve));
  udp.endPacket();

  int while_counter = 0;
  uint64_t start_time = micros();
  while(current_angle < angle) {
    while_counter++;

    // Abbruchkriterium, falls einer der Driftwerte > pi
    if (computed_drifts.x > M_PI || computed_drifts.y > M_PI || computed_drifts.z > M_PI) {
      Serial.printf("Drift: (%f %f %f)\n", computed_drifts.x, computed_drifts.y, computed_drifts.z);
      break;
    }
    update_orientation(computed_drifts);
    motors::set_linear_velocities(v_l, v_r);
    current_angle = calculateRotationAngle(q_initial, orientation);
    delay(1);
  }
  uint64_t end_time = micros();
  auto time_diff = end_time - start_time;

  // nach der Kurve senden an david f-ffff-int-long-int
  udp.beginPacket(IPAddress(10, 0, 2, 137), 4444);
  udp.write((uint8_t *) &current_angle, sizeof(current_angle));
  udp.write((uint8_t *) &orientation, sizeof(orientation));
  udp.write((uint8_t *) &while_counter, sizeof(while_counter));
  udp.write((uint8_t *) &time_diff, sizeof(time_diff));
  udp.write((uint8_t *) &end_curve, sizeof(end_curve));
  udp.endPacket();

  motors::set_linear_velocities(0, 0);
}

void drive_curve_integral(murmecha::math::Vector3 computed_drifts,float radius, float angle, float vm) {
  int start_curve = 1;
  int end_curve = 0;

  // Ausrichtung zurücksetzen
  float current_angle = 0; 

  // R_l mit + --> rechtskurve, R_r mit - --> linkskurve
  // R_l mit - --> rechtskurve, R_r mit + --> linkskurve
  float R_l = radius - R_perp;
  float R_r = radius + R_perp;

  float v_r = 2 * vm / (1 + (R_l / R_r));
  float v_l = v_r * (R_l / R_r);

  // vor der Kurve senden an david f-ffff-int
  udp.beginPacket(IPAddress(10, 0, 2, 137), 4444);
  udp.write((uint8_t *) &current_angle, sizeof(current_angle));
  udp.write((uint8_t *) &start_curve, sizeof(start_curve));
  udp.endPacket();

  int while_counter = 0;
  uint64_t start_time = micros();
  while(current_angle < angle) {
    while_counter++;
    // hier kommt äquivalent zu timed update ohne madgwick rein
    motors::set_linear_velocities(v_l, v_r);
    // berechne current angle
    delay(1);
  }
  uint64_t end_time = micros();
  auto time_diff = end_time - start_time;

  // nach der Kurve senden an david f-ffff-int-long-int
  udp.beginPacket(IPAddress(10, 0, 2, 137), 4444);
  udp.write((uint8_t *) &current_angle, sizeof(current_angle));
  udp.write((uint8_t *) &while_counter, sizeof(while_counter));
  udp.write((uint8_t *) &time_diff, sizeof(time_diff));
  udp.write((uint8_t *) &end_curve, sizeof(end_curve));
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

  drive_segment(250.0f, vel);

  // Ecke fahren
  // drive_curve_madgwick(computed_drifts, 0, M_PI/2, vel);

  // runde Ecke fahren
  drive_curve_madgwick(computed_drifts, 100, M_PI/2, vel);

  murmecha::display::clear();
  murmecha::display::draw_info_screen(16);
  murmecha::display::draw_tracking_code(16);
  murmecha::display::update();
}