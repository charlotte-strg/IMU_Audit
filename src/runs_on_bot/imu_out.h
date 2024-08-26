#ifndef IMU_OUT_H
#define IMU_OUT_H

#include <Arduino.h>
#include <robot.hpp>
#include <imu.hpp>
#include <cmath> // Für die ceil-Funktion
#include "imu_data.h"

void custom_setup();
void custom_loop_position();
void custom_loop();

#endif
