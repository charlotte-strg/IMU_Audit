#include <Arduino.h>
// #include <imu_serial_out.h>
// #include "imu_out.h"
#include "square.h"

void setup() {
  // custom_setup();
  custom_setup_square();
}

void loop() {
  //custom_loop();
  // custom_loop_position();
  custom_loop_square();
}