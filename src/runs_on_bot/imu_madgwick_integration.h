#ifndef IMU_MADGWICK_INTEGRATION_HPP
#define IMU_MADGWICK_INTEGRATION_HPP

#include <Arduino.h>
#include <imu.hpp>
#include "madgwick.h"
#include <robot.hpp>
#include <array>
#include "quaternions.h"

extern Quaternion orientation_madgwick;

void IRAM_ATTR update_orientation_madgwick(murmecha::math::Vector3 computed_drifts);
void reset_orientation_madgwick();

#endif
