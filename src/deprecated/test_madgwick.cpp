// #include <Arduino.h>
// #include <robot.hpp>
// #include <rgb.hpp>
// #include <uv.hpp>
// #include <display.hpp>
// #include <motors.hpp>
// #include <pins.hpp>
// #include <WiFi.h>
// #include <driver/mcpwm.h>
// #include "imu_madgwick_integration.h"

// #define bufferSize 256

// using namespace murmecha;

// //Wifi Setup
// extern WiFiUDP udp;


// void custom_setup_test_madgwick() {
//   Serial.begin(115200);
//   murmecha::config_t config;
//   config.use_microstepping = false;
//   murmecha::begin(config);
//   // murmecha::uv::set(1.0f);
//   // murmecha::rgb::set_led_color(0, 64, 0, 0);
//   murmecha::display::invert();
//   murmecha::display::clear();
//   murmecha::display::draw_tracking_code(16);
//   murmecha::display::draw_info_screen(16);
//   murmecha::display::update();

// //IP-Adresse (6 für bot mit nummer 6), Gateway, an wen senden (zuhause)
// WiFi.config(IPAddress(10, 0, 1, 22), IPAddress(10, 0, 2, 1), IPAddress(255, 255, 0, 0));
// Insert your WiFi credentials here:
// String network;
// String pass_key;

// //WLAN, Passwort
// WiFi.begin(network, pass_key);
// while (!WiFi.isConnected()){
//   delay(100);
//   Serial.println("Connecting to WiFi..");
// }
// //4444 port auf meinem Laptop
// udp.begin(4444);


// murmecha::motors::beep(1200,500);

// delay(2000);
// // pinMode(RIGHT_MOTOR_STEP, OUTPUT);
// }

// void custom_loop_test_madgwick() {
// while(true) {
//     update_orientation(computed_drifts);

// //senden an david
// udp.beginPacket(IPAddress(10, 0, 2, 137), 4444);
// //udp.write((uint8_t *) &current_angle, sizeof(current_angle));
// udp.write((uint8_t *) &orientation, sizeof(orientation));
// udp.endPacket();
// }

// murmecha::display::clear();
// murmecha::display::draw_info_screen(16);
// murmecha::display::draw_tracking_code(16);
// murmecha::display::update();
// }