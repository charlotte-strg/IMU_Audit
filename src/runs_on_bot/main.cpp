#include <Arduino.h>
// #include <imu_serial_out.h>
#include "imu_out.h"

void setup() {
  custom_setup();
}

void loop() {
  //custom_loop();
  custom_loop_position();
}