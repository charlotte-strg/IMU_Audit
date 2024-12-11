#ifndef FILTER_HPP
#define FILTER_HPP

#include <cmath>
#include <array>
#include "quaternions.h"

// System constants
constexpr float deltat = 0.001f;  // Sampling period in seconds (1 ms)
constexpr float gyroMeasError = M_PI * (5.0f / 180.0f);  // Gyroscope measurement error in rad/s (5 deg/s)
const float beta = std::sqrt(3.0f / 4.0f) * gyroMeasError;  // Compute beta

// Function to update the quaternion based on sensor data
Quaternion filterUpdate(
    const Quaternion& q,
    float w_x, float w_y, float w_z,
    float a_x, float a_y, float a_z
);

#endif // FILTER_HPP