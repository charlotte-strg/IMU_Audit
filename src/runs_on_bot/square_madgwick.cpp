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
// float R_perp = 37.0f;
// für bot 16 und 22
float R_perp = 68.0f/2.0f;

//Wifi Setup
extern WiFiUDP udp;


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

//IP-Adresse (6 für bot mit nummer 6), Gateway, an wen senden (zuhause)
WiFi.config(IPAddress(10, 0, 1, 22), IPAddress(10, 0, 2, 1), IPAddress(255, 255, 0, 0));
//WLAN, Passwort (UNI)
WiFi.begin("DeziWLAN", "Ingwer-Zitrone");
while (!WiFi.isConnected()){
  delay(100);
  Serial.println("Connecting to WiFi..");
}
//4444 port auf meinem Laptop
udp.begin(4444);


  murmecha::motors::beep(1200,500);


  // wichtig: hier einfügen: offset kalibrierung für das gyroskop only
  // durchschnittlichen drift/offset ermitteln für alle achsen
  // drift/offset jeder achse von sich selbst abziehen, um alle zu nullen


  delay(2000);
  // pinMode(RIGHT_MOTOR_STEP, OUTPUT);

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

// kurve fahren mit madgwick, muss noch in namen eingebaut werden
void drive_curve(float radius, float angle, float vm) {
    // float dist = radius * angle;
    // float time = dist / vm;

    reset_orientation();
    Quaternion q_initial = Quaternion::identity();
    float current_angle = 0;

    float R_l = radius + R_perp;
    float R_r = radius - R_perp;

    float v_r = 2 * vm / (1 + (R_l / R_r));
    float v_l = v_r * (R_l / R_r);

    // delay(time * 1000);
    int while_counter = 0;
    uint64_t start_time = micros();
    while(current_angle < angle) {
      while_counter++;
      timedUpdate();
      //drive_circle(radius, vm);
      motors::set_linear_velocities(v_l, v_r);
      current_angle = calculateRotationAngle(q_initial, orientation);
      delay(1);
    }
    uint64_t end_time = micros();
    auto time_diff = end_time - start_time;
    //senden an david
    udp.beginPacket(IPAddress(10, 0, 2, 137), 4444);
    //udp.write((uint8_t *) &current_angle, sizeof(current_angle));
    udp.write((uint8_t *) &orientation, sizeof(orientation));
    udp.write((uint8_t *) &while_counter, sizeof(while_counter));
    udp.write((uint8_t *) &time_diff, sizeof(time_diff));
    udp.endPacket();

  motors::set_linear_velocities(0, 0);
}

void custom_loop_square_madgwick() {
  drive_segment(250.0f, vel);
  drive_curve(100, M_PI, vel);

  murmecha::display::clear();
  murmecha::display::draw_info_screen(16);
  murmecha::display::draw_tracking_code(16);
  murmecha::display::update();
}