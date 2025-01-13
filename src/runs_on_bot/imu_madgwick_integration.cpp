#include <Arduino.h>
#include <imu.hpp>
#include "madgwick.h"
#include <robot.hpp>
#include "quaternions.h"

// quaternionen global initialisiert mit 1, 0, 0, 0
Quaternion orientation = Quaternion::identity();

// aktualisiert orientierung des bots mit jedem neuen datenpunkt aus der IMU
void IRAM_ATTR timedUpdate(murmecha::math::Vector3 computed_drifts) {
// attribut IRAM_ATTR --> timedUpdate im schnellen arbeitsspeicher
    murmecha::imu::read_accel_and_gyro();
    auto accel = murmecha::imu::get_acceleration();
    auto gyro = murmecha::imu::get_gyroscope();

    murmecha::math::Vector3 computed_gyro = gyro - computed_drifts;

    orientation = filterUpdate(
        orientation,
        computed_gyro.x, computed_gyro.y, computed_gyro.z,
        accel.x, accel.y, accel.z
    );
}

// quaternionen zurücksetzen
void reset_orientation() {
    orientation = Quaternion::identity();
}