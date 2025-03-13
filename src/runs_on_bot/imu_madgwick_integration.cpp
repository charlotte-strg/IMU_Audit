#include <Arduino.h>
#include <imu.hpp>
#include "madgwick.h"
#include <robot.hpp>
#include "quaternions.h"

// quaternion global initialisiert mit 1, 0, 0, 0
Quaternion orientation_madgwick = Quaternion::identity();

// aktualisiert orientierung des bots mit jedem neuen datenpunkt aus der IMU
void IRAM_ATTR update_orientation_madgwick(murmecha::math::Vector3 computed_drifts) {
// attribut IRAM_ATTR --> update_orientation_madgwick im schnellen arbeitsspeicher
    murmecha::imu::read_accel_and_gyro();
    auto accel = murmecha::imu::get_acceleration();
    auto gyro = murmecha::imu::get_gyroscope();

    murmecha::math::Vector3 computed_gyro = gyro - computed_drifts;

    orientation_madgwick = madgwick_filter(
        orientation_madgwick,
        computed_gyro.x, computed_gyro.y, computed_gyro.z,
        accel.x, accel.y, accel.z
    );
}

// quaternionen zurücksetzen
void reset_orientation_madgwick() {
    orientation_madgwick = Quaternion::identity();
}