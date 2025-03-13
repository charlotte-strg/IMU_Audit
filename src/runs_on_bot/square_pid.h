#ifndef SQUARE_MADGWICK_H
#define SQUARE_MADGWICK_H

#include <Arduino.h>
#include <WiFi.h>
#include <murmecha.hpp>
#include "imu_madgwick_integration.h"
#include <tuple>

extern murmecha::math::Vector3 computed_drifts;
murmecha::math::Vector3 measure_gyro_drift();
float pid_control(float target_rad, float current_rad);
extern Quaternion q_initial_pid;

extern float target_angle;
extern WiFiUDP udp;
extern TaskHandle_t pid_task_handle;
extern TaskHandle_t madgwick_task_handle;

void update_madgwick(void *args);
void update_pid(void *args);

struct accel_gyro_data_t {
    murmecha::math::Vector3 acc;
    murmecha::math::Vector3 gyro;
};

extern const uint32_t imu_sample_count = 2048;
extern accel_gyro_data_t imu_samples[imu_sample_count];

murmecha::math::Vector3 get_drift();
void setup_square_pid();
void loop_square_pid();

std::tuple<float, float> pid_to_wheel_speeds(float pid_output, float base_speed);

#endif // SQUARE_MADGWICK_H