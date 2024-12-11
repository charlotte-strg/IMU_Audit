#include <Arduino.h>

#define MURMECHA_REV1_MOTOR_UPGRADE

#include <robot.hpp>
#include <rgb.hpp>
#include <uv.hpp>
#include <display.hpp>
#include <motors.hpp>
#include <pins.hpp>

#include <driver/mcpwm.h>

#include "imu_madgwick_integration.h"

using namespace murmecha;

float vel = 60.0f;
// float R_perp = 37.0f;
// für bot 16
float R_perp = 68.0f/2.0f;

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
  murmecha::motors::beep(1200,500);

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

void drive_curve(float radius, float angle, float vm) {
    // float dist = radius * angle;
    // float time = dist / vm;

    reset_orientation();
    Quaternion q_initial = Quaternion::identity();
    float current_angle = 0;

    drive_circle(radius, vm);

    // delay(time * 1000);
    // delay ersetzen durch schleife
    // in schleife konstant orientierung überprüfen, bis 90 grad drehung erreicht ist
    while(current_angle < angle) {
      timedUpdate();
      current_angle = calculateRotationAngle(q_initial, orientation);
      delay(1);
    }

  motors::set_linear_velocities(0, 0);
}

void custom_loop_square_madgwick() {
  drive_segment(250.0f, vel);
  drive_curve(100, M_PI / 2, vel);

  murmecha::display::clear();
  murmecha::display::draw_info_screen(16);
  murmecha::display::draw_tracking_code(16);
  murmecha::display::update();

  Serial.println(vel);
}