#include <Arduino.h>
// #include <imu_serial_out.h>
#include "imu_out.h"

void setup() {
  custom_setup();
}

void loop() {
  custom_loop();
}


/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/vs-code-platformio-ide-esp32-esp8266-arduino/
*********/

// #include <Arduino.h>
// #include <robot.hpp>
// #include <imu.hpp>

// #define LED 21
// unsigned long startTime; // Variable to store the start timeunsigned long startTime; // Variable to store the start time

// void setup() {
//   Serial.begin(115200);
//   murmecha::begin();
//   startTime = millis(); // Record the start time
// }

// void loop() {
//   unsigned long currentTime = millis(); // Get the current time
  
//   // Check if one minute (60000 milliseconds) has passed
//   if (currentTime - startTime >= 60000) {
//     // Stop further execution
//     return;
//   }
//   // put your main code here, to run repeatedly:
//   murmecha::imu::read_accel_and_gyro();
//   auto accel = murmecha::imu::get_acceleration();
//   auto gyro = murmecha::imu::get_gyroscope();
//   Serial.printf("Accel: %f %f %f \r\n", accel.x, accel.y, accel.z);
//   Serial.printf("Gyro: %f %f %f \r\n", gyro.x, gyro.y, gyro.z);
//   // Serial.printf("%f,%f,%f,", accel.x, accel.y, accel.z);
//   // Serial.printf("%f,%f,%f\n", gyro.x, gyro.y, gyro.z);
//   delay(1000);
// }