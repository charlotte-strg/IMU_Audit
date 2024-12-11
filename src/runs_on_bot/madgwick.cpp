// #include <iostream>
#include <cmath>
#include <array>
#include "quaternions.h"


// System constants
constexpr float deltat = 0.001f; // Sampling period in seconds (1 ms)
constexpr float gyroMeasError = M_PI * (5.0f / 180.0f); // Gyroscope measurement error in rad/s (5 deg/s)
const float beta = std::sqrt(3.0f / 4.0f) * gyroMeasError; // Compute beta

// Global system variables
// float SEq_1 = 1.0f, SEq_2 = 0.0f, SEq_3 = 0.0f, SEq_4 = 0.0f; // Estimated orientation quaternion elements

Quaternion filterUpdate(const Quaternion& q, float w_x, float w_y, float w_z, float a_x, float a_y, float a_z) {
    // Local system variables
    float norm; // Vector norm
    float SEqDot_omega_1, SEqDot_omega_2, SEqDot_omega_3, SEqDot_omega_4; // Quaternion derivative from gyroscope elements
    float f_1, f_2, f_3; // Objective function elements
    float J_11or24, J_12or23, J_13or22, J_14or21, J_32, J_33;  // Objective function Jacobian elements
    float SEqHatDot_1, SEqHatDot_2, SEqHatDot_3, SEqHatDot_4;  // Estimated direction of gyroscope error

    // Auxiliary variables to avoid repeated calculations
    const float halfSEq_1 = 0.5f * q.w;
    const float halfSEq_2 = 0.5f * q.x;
    const float halfSEq_3 = 0.5f * q.y;
    const float halfSEq_4 = 0.5f * q.z;
    const float twoSEq_1 = 2.0f * q.w;
    const float twoSEq_2 = 2.0f * q.x;
    const float twoSEq_3 = 2.0f * q.y;

    // Normalize the accelerometer measurement
    norm = std::sqrt(a_x * a_x + a_y * a_y + a_z * a_z);
    a_x /= norm;
    a_y /= norm;
    a_z /= norm;

    // Compute the objective function and Jacobian
    f_1 = twoSEq_2 * q.z - twoSEq_1 * q.y - a_x;
    f_2 = twoSEq_1 * q.x + twoSEq_3 * q.z - a_y;
    f_3 = 1.0f - twoSEq_2 * twoSEq_2 - twoSEq_3 * twoSEq_3 - a_z;
    J_11or24 = twoSEq_3;
    J_12or23 = 2.0f * q.z;
    J_13or22 = twoSEq_1;
    J_14or21 = twoSEq_2;
    J_32 = 2.0f * J_14or21;
    J_33 = 2.0f * J_11or24;

    // Compute the gradient (matrix multiplication)
    SEqHatDot_1 = J_14or21 * f_2 - J_11or24 * f_1;
    SEqHatDot_2 = J_12or23 * f_1 + J_13or22 * f_2 - J_32 * f_3;
    SEqHatDot_3 = J_12or23 * f_2 - J_33 * f_3 - J_13or22 * f_1;
    SEqHatDot_4 = J_14or21 * f_1 + J_11or24 * f_2;

    // Normalize the gradient
    norm = std::sqrt(SEqHatDot_1 * SEqHatDot_1 + SEqHatDot_2 * SEqHatDot_2 +
                     SEqHatDot_3 * SEqHatDot_3 + SEqHatDot_4 * SEqHatDot_4);
    SEqHatDot_1 /= norm;
    SEqHatDot_2 /= norm;
    SEqHatDot_3 /= norm;
    SEqHatDot_4 /= norm;

    // Compute the quaternion derivative measured by gyroscopes
    SEqDot_omega_1 = -halfSEq_2 * w_x - halfSEq_3 * w_y - halfSEq_4 * w_z;
    SEqDot_omega_2 = halfSEq_1 * w_x + halfSEq_3 * w_z - halfSEq_4 * w_y;
    SEqDot_omega_3 = halfSEq_1 * w_y - halfSEq_2 * w_z + halfSEq_4 * w_x;
    SEqDot_omega_4 = halfSEq_1 * w_z + halfSEq_2 * w_y - halfSEq_3 * w_x;

    // Compute and integrate the estimated quaternion derivative
    float SEq_1_new = q.w + (SEqDot_omega_1 - (beta * SEqHatDot_1)) * deltat;
    float SEq_2_new = q.x + (SEqDot_omega_2 - (beta * SEqHatDot_2)) * deltat;
    float SEq_3_new = q.y + (SEqDot_omega_3 - (beta * SEqHatDot_3)) * deltat;
    float SEq_4_new = q.z + (SEqDot_omega_4 - (beta * SEqHatDot_4)) * deltat;

    // Normalize quaternion
    norm = std::sqrt(SEq_1_new * SEq_1_new + SEq_2_new * SEq_2_new + SEq_3_new * SEq_3_new + SEq_4_new * SEq_4_new);
    SEq_1_new /= norm;
    SEq_2_new /= norm;
    SEq_3_new /= norm;
    SEq_4_new /= norm;
    return {SEq_1_new, SEq_2_new, SEq_3_new, SEq_4_new};
}
