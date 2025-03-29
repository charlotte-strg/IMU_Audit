#include <Arduino.h>
#include "square.h"
#include "square_pid.h" 

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