#ifndef FILTER_H
#define FILTER_H

// Math library required for sqrt
#include <math.h>

// System constants
#define deltat 0.001f // sampling period in seconds (shown as 1 ms)
#define gyroMeasError 3.14159265358979f * (5.0f / 180.0f) // gyroscope measurement error in rad/s (shown as 5 deg/s)
#define beta sqrt(3.0f / 4.0f) * gyroMeasError // compute beta

// Global system variables (extern to be accessed by other files)
extern float a_x, a_y, a_z; // accelerometer measurements
extern float w_x, w_y, w_z; // gyroscope measurements in rad/s
extern float SEq_1, SEq_2, SEq_3, SEq_4; // estimated orientation quaternion elements with initial conditions

// Function prototype for filter update
void filterUpdate(float w_x, float w_y, float w_z, float a_x, float a_y, float a_z);

#endif // FILTER_H
