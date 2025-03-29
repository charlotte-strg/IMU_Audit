#ifndef MURMECHA_CONTROLLER_H
#define MURMECHA_CONTROLLER_H

#include <Arduino.h>
#include <robot.hpp>
#include <rgb.hpp>
#include <uv.hpp>
#include <display.hpp>
#include <motors.hpp>
#include <pins.hpp>
#include <driver/mcpwm.h>
#include "imu_madgwick_integration.h"
#include "imu_integral_integration.h"
#include <tuple>

void setup_square();
void loop_square();

namespace murmecha {

// Konfigurationsstruktur für die Initialisierung
// struct config_t {
//     bool use_microstepping = false;
// };
// struct config_t_square {
//     bool use_microstepping = false;
// };

// Initialisierung
void begin(const config_t& config);
// void begin(const config_t_square& config);

// Display-Steuerung
namespace display {
    void invert();
    void clear();
    void draw_tracking_code(int size);
    void draw_info_screen(int size);
    void update();
}

// Motorsteuerung
namespace motors {
    void set_linear_velocities(float v_left, float v_right);
}

// UV- und RGB-Steuerung (Falls benötigt, einkommentieren)
// namespace uv {
//     void set(float intensity);
// }
// namespace rgb {
//     void set_led_color(uint8_t led, uint8_t r, uint8_t g, uint8_t b);
// }

// Bewegungsfunktionen

// void drive_circle(float radius, float velocity);
// void drive_segment(float length, float velocity);
// void drive_curve_madgwick(float radius, float angle, float velocity);
// void drive_curve_integral(float radius, float angle, float velocity);

murmecha::math::Vector3 measure_gyro_drift();
void drive_segment_motors(float length, float vm);
void drive_circle_motors(float radius, float vm);
void drive_curve_motors(float radius, float angle, float vm);
void drive_curve_integral(murmecha::math::Vector3 computed_drifts, float radius, float angle, float vm);
void drive_curve_madgwick(murmecha::math::Vector3 computed_drifts, float radius, float target_angle, float vm);
float pid_control(float target_angle, float current_angle);
void reset_pid();
// std::tuple<float, float> pid_to_wheel_speeds(float pid_output, float base_speed);

} // namespace murmecha

#endif // MURMECHA_CONTROLLER_H
