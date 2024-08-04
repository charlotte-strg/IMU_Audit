#include <Arduino.h>
#include <robot.hpp>
#include <imu.hpp>
#include <WiFi.h>
// #include <cmath> // Für die ceil-Funktion
#include "imu_data.h"

#define LED 21
// #define bufferSize 1024 // geändert von 1000
#define bufferSize 256

// parameter for manual change
float recordingTimeInSec = 1; 
// unsigned long delayInMillis = 1;

// fixed parameters
float recordingTimeInMillis; 

unsigned long startTime; // Variable to store the unsigned long startTime
unsigned long currentTime; // time at the start of the sampling process / starting point of custom loop function
// unsigned long oldTime;
// unsigned long sampleDeltaTime; // time passed since last sample
unsigned long startDeltaTime; // time passed since start

IMUData buffer[bufferSize]; // initialize buffer at fixed size
// int bufferIndex = 0;

WiFiUDP udp;



void custom_setup() {
    Serial.begin(115200);
    murmecha::begin();
    //delay(5000);
    recordingTimeInMillis = (unsigned long) recordingTimeInSec * 1000;

    //IP-Adresse (6 für bot mit nummer 6), Gateway, an wen senden (zuhause)
    WiFi.config(IPAddress(10, 0, 1, 6), IPAddress(10, 0, 2, 1), IPAddress(255, 255, 0, 0));

    //WLAN, Passwort (zuhause)
    WiFi.begin("Vodafone-2584", "4EgXXA7R93x9ypCz");
    
    //WLAN, Passwort (UNI)
    // WiFi.begin("DeziWLAN", "Ingwer-Zitrone");

    //WLAN, Passwort (ZackZack)
    // WiFi.begin("ESPRESSOZACKZACK", "WIFIZACKZACK");

    while (!WiFi.isConnected()){
        delay(100);
        Serial.println("Connecting to WiFi..");
    }
    //4444 port auf meinem Laptop
    udp.begin(4444);


    startTime = micros(); // Record the start time
    currentTime = startTime; //

}

void custom_loop() {

    currentTime = micros();

    // sampleDeltaTime = currentTime - oldTime;
    startDeltaTime = currentTime - startTime;
    murmecha::imu::read_accel_and_gyro();
    auto accel = murmecha::imu::get_acceleration();
    auto gyro = murmecha::imu::get_gyroscope();
    auto entry = IMUData(startDeltaTime, accel, gyro);

    
    // for (int i = 0; i < bufferSize; i++){
    //     // oldTime = currentTime;
    //     currentTime = micros();

    //     // sampleDeltaTime = currentTime - oldTime;
    //     startDeltaTime = currentTime - startTime;

    //     murmecha::imu::read_accel_and_gyro();
    //     auto accel = murmecha::imu::get_acceleration();
    //     auto gyro = murmecha::imu::get_gyroscope();
    //     buffer[i] = IMUData(startDeltaTime, accel, gyro);
    //     delay(1);
    //}

    // Durchlaufe den Buffer und printe jeden Eintrag als Zeile
    // for (int i = 0; i < bufferSize; i++) {
    //     // Eintrag im Buffer, Referenz mit &
    //     IMUData entry = buffer[i];
    //     Serial.printf("[%d,%d,%f,%f,%f,%f,%f,%f]\n", i, entry.id, entry.accel.x, entry.accel.y, entry.accel.z, entry.gyro.x, entry.gyro.y, entry.gyro.z);
    //     delay(10);
    // }

    // sende den Buffer über UDP
    // IP-Adresse 10, 0, 2 gesetzt, letzte Zahl kann gewählt werden außer 1, 42, 29, 137)
    udp.beginPacket(IPAddress(10, 0, 2, 11), 4444);

    udp.write((uint8_t *) &entry, sizeof(IMUData));
    udp.endPacket();    
}