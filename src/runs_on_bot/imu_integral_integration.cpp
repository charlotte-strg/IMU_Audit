#include <Arduino.h>
#include <imu.hpp>
#include "madgwick.h"
#include <robot.hpp>
#include "quaternions.h"

murmecha::math::Vector3 orientation_integral = {0.0f, 0.0f, 0.0f};

// aktualisiert orientierung des bots mit jedem neuen datenpunkt aus der IMU
void IRAM_ATTR update_orientation_integral(murmecha::math::Vector3 computed_drifts) {
// attribut IRAM_ATTR --> update_orientation_integrals im schnellen arbeitsspeicher
    
    // samplingzeit analog zu madgwick
    constexpr float deltat = 1.034e-3;
    
    murmecha::imu::read_accel_and_gyro();
    auto accel = murmecha::imu::get_acceleration();
    auto gyro = murmecha::imu::get_gyroscope();

    murmecha::math::Vector3 computed_gyro = gyro - computed_drifts;

    // david: bitte nochmal * als operator definieren (skalarprodukt)
    computed_gyro.x = computed_gyro.x * deltat;
    computed_gyro.y = computed_gyro.y * deltat;
    computed_gyro.z = computed_gyro.z * deltat;
    orientation_integral = orientation_integral + computed_gyro;
}    

// orientierung zurücksetzen
void reset_orientation_integral() {
    orientation_integral = {0.0f, 0.0f, 0.0f};
}