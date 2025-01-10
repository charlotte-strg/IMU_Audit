#include <Arduino.h>
// #include <imu_serial_out.h>
// #include "imu_out.h"
// #include "square_david.h"
#include "square_madgwick.h"
// #include  "test_madgwick.h"

void setup() {
  // custom_setup();
  // custom_setup_square_david();
  custom_setup_square_madgwick();
  // custom_setup_test_madgwick();
}

void loop() {
  // custom_loop();
  // custom_loop_position();
  // custom_loop_square_david();
  custom_loop_square_madgwick();
  // custom_loop_test_madgwick();
}