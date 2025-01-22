#ifndef IMU_INTEGRAL_INTEGRATION_HPP
#define IMU_INTEGRAL_INTEGRATION_HPP

#include <Arduino.h>
#include <imu.hpp>
#include <robot.hpp>
#include <array>

extern murmecha::math::Vector3 orientation_integral;

void IRAM_ATTR update_orientation_integral(murmecha::math::Vector3 computed_drifts);
void reset_orientation_integral();

#endif