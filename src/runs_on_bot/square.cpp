#include <Arduino.h>

#define MURMECHA_REV1_MOTOR_UPGRADE

#include <robot.hpp>
#include <rgb.hpp>
#include <uv.hpp>
#include <display.hpp>
#include <motors.hpp>
#include <pins.hpp>

#include <driver/mcpwm.h>

using namespace murmecha;

void custom_setup_square() {
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

  delay(2000);
  // pinMode(RIGHT_MOTOR_STEP, OUTPUT);
}

float vel = 60.0f;
// float R_perp = 37.0f;
// für bot 16
float R_perp = 68.0f/2.0f;

void drive_circle(float radius, float vm) {

  float R_l = radius - R_perp;
  float R_r = radius + R_perp;

  float v_r = 2 * vm / (1 + (R_l / R_r));
  float v_l = v_r * (R_l / R_r);
  //v_r habe ich umgedreht, weil rad falsch montiert
  murmecha::motors::set_linear_velocities(v_l, -v_r);

}

void drive_segment(float length, float vm) {

  float time = length / vm;
  motors::set_linear_velocities(vm, -vm);
  delay(time*1000);
  motors::set_linear_velocities(0, 0);

}

void drive_curve(float radius, float angle, float vm) {
  // aktuelle orientierung zwischenspeichern

  float dist = radius * angle;
  float time = dist / vm;

  drive_circle(radius, vm);

  delay(time * 1000);
  // delay ersetzen durch schleife
  // in schleife konstant orientierung überprüfen, bis 90 grad drehung erreicht ist

  motors::set_linear_velocities(0, 0);

}

void custom_loop_square() {
  // motors::set_rotation_speeds(M_PI, M_PI);
  // motors::set_linear_velocities(vel, -vel);

  // drive_circle(150.0f, 30.0f);

  drive_segment(250.0f, vel);
  drive_curve(100, M_PI / 2, vel);

  murmecha::display::clear();
  murmecha::display::draw_info_screen(16);
  murmecha::display::draw_tracking_code(16);
  murmecha::display::update();
  // vel *= 1.2;
  // for (int i = 0; i < 380; ++i) {
  //   digitalWrite(RIGHT_MOTOR_STEP, 1);
  //   delay(1);
  //   digitalWrite(RIGHT_MOTOR_STEP, 0);
  //   delay(1);
  // }
  Serial.println(vel);
  // delay(2000);
}