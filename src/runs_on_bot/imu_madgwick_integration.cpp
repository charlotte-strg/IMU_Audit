#include <Arduino.h>
#include <imu.hpp>  // Annahme: murmecha::imu wird verwendet
#include "madgwick.h" // Madgwick-Funktion
#include <robot.hpp>

// Globale Variablen für die Orientierung (Quaternionen)
float SEq_1 = 1.0f, SEq_2 = 0.0f, SEq_3 = 0.0f, SEq_4 = 0.0f;

// void setup() {
//     Serial.begin(115200);
//     murmecha::begin();
//     delay(5000); // Zeit für IMU-Initialisierung
//     Serial.println("imu_madgwick_integration: Setup done\n");
// }

void custom_loop_m_int() {
    // Daten von der IMU lesen
    murmecha::imu::read_accel_and_gyro();
    auto accel = murmecha::imu::get_acceleration(); // Beschleunigung auslesen
    auto gyro = murmecha::imu::get_gyroscope();    // Gyroskop-Daten auslesen

    // Madgwick-Filter aufrufen
    std::array<float, 4> result = filterUpdate(SEq_1, SEq_2, SEq_3, SEq_4, gyro.x, gyro.y, gyro.z, accel.x, accel.y, accel.z);
    SEq_1 = result[0];
    SEq_2 = result[1];
    SEq_3 = result[2];
    SEq_4 = result[3];

    // // Orientierung ausgeben
    // Serial.printf("Quaternion: [%f, %f, %f, %f]\n", SEq_1, SEq_2, SEq_3, SEq_4);

    // Optional: eine kleine Verzögerung
    delay(10); // 100 Hz Abtastrate
}
