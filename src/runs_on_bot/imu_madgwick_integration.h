#ifndef IMU_MADGWICK_INTEGRATION_HPP
#define IMU_MADGWICK_INTEGRATION_HPP

#include <Arduino.h>
#include <imu.hpp>
#include "madgwick.h"
#include <robot.hpp>
#include <array>
#include "quaternions.h"

extern Quaternion orientation;

void IRAM_ATTR timedUpdate();
void reset_orientation();

#endif
