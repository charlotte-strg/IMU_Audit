#ifndef IMU_MADGWICK_INTEGRATION_HPP
#define IMU_MADGWICK_INTEGRATION_HPP

#include <Arduino.h>
#include <imu.hpp>  // Annahme: murmecha::imu wird verwendet
#include "madgwick.h" // Madgwick-Funktion
#include <robot.hpp>
#include <array>

// Globale Variablen für die Orientierung (Quaternionen)
extern float SEq_1;
extern float SEq_2;
extern float SEq_3;
extern float SEq_4;

// Funktionsdeklarationen
// void setup();
void custom_loop_m_int();

#endif // IMU_MADGWICK_INTEGRATION_HPP
