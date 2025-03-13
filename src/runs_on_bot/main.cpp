#include <Arduino.h>
#include "square_madgwick.h"
#include "square_pid.h"

// deprecated:
// #include  "test_madgwick.h"
// #include <imu_serial_out.h>
// #include "imu_out.h"
// #include "square_david.h"
 

void setup() {
  // Setup für alle Steuerungsmodelle außer PID
  // setup_square();

  // Setup für PID-Steuerung
  setup_square_pid();
}

void loop() {
  // Loop für alle Steuerungsmodelle außer PID
  // loop_square();

  // Loop für PID-Steuerung
  loop_square_pid();
}