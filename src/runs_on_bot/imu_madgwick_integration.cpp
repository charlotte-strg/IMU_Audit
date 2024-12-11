#include <Arduino.h>
#include <imu.hpp>
#include "madgwick.h"
#include <robot.hpp>
#include "quaternions.h"

// Globale Variablen für die Orientierung (Quaternionen)
Quaternion orientation = Quaternion::identity();

// IRAM_ATTR sorgt dafür, dass timedUpdate im schnellen arbeitsspeicher gehalten wird
void IRAM_ATTR timedUpdate() {
    murmecha::imu::read_accel_and_gyro();
    auto accel = murmecha::imu::get_acceleration();
    auto gyro = murmecha::imu::get_gyroscope();

    orientation = filterUpdate(
        orientation,
        gyro.x, gyro.y, gyro.z,
        accel.x, accel.y, accel.z
    );
}

void reset_orientation() {
    orientation = Quaternion::identity();
}