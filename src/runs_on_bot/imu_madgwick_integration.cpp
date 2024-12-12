#include <Arduino.h>
#include <imu.hpp>
#include "madgwick.h"
#include <robot.hpp>
#include "quaternions.h"

// quaternionen global initialisiert mit 1, 0, 0, 0
Quaternion orientation = Quaternion::identity();

// aktualisiert orientierung des bots mit jedem neuen datenpunkt aus der IMU
void IRAM_ATTR timedUpdate() {
// attribut IRAM_ATTR --> timedUpdate im schnellen arbeitsspeicher
    murmecha::imu::read_accel_and_gyro();
    auto accel = murmecha::imu::get_acceleration();
    auto gyro = murmecha::imu::get_gyroscope();

    orientation = filterUpdate(
        orientation,
        gyro.x, gyro.y, gyro.z,
        accel.x, accel.y, accel.z
    );
}

// quaternionen zurücksetzen
void reset_orientation() {
    orientation = Quaternion::identity();
}