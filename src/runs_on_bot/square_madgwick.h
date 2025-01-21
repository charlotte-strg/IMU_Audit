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

void custom_setup_square_madgwick();
void custom_loop_square_madgwick();

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
void drive_circle(float radius, float velocity);
void drive_segment(float length, float velocity);
void drive_curve_madgwick(float radius, float angle, float velocity);
void drive_curve_integral(float radius, float angle, float velocity);

} // namespace murmecha

#endif // MURMECHA_CONTROLLER_H
