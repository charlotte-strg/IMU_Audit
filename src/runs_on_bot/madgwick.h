#ifndef FILTER_HPP
#define FILTER_HPP

#include <cmath>
#include <array>
#include "quaternions.h"

// System constants
// constexpr float deltat = 0.001f;  // sampling period in sec
// constexpr float gyroMeasError = M_PI * (5.0f / 180.0f);  // gyro messfehler in rad/s
// const float beta = std::sqrt(3.0f / 4.0f) * gyroMeasError;  // berechne beta

// applies madgwick filter to sensor data, returns quaternion
Quaternion madgwick_filter(
    const Quaternion& q,
    float w_x, float w_y, float w_z,
    float a_x, float a_y, float a_z
);

#endif // FILTER_HPP
