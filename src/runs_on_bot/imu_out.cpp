#include <Arduino.h>
#include <robot.hpp>
#include <imu.hpp>
#include <WiFi.h>
#include <display.hpp>
#include <motors.hpp>
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
unsigned long startDeltaTime; // time passed since start

IMUData buffer[bufferSize]; // initialize buffer at fixed size
// int bufferIndex = 0;

WiFiUDP udp;

// Position des Roboters (initial auf 0 gesetzt)
float posX = 0.0;
float posY = 0.0;
float posZ = 0.0;

// Variablen für Geschwindigkeit (initial auf 0 gesetzt)
float velocityX = 0.0;
float velocityY = 0.0;
float velocityZ = 0.0;

unsigned long loopCounter = 0;
unsigned long previousTime = 0;


void custom_setup() {
    Serial.begin(115200);
    murmecha::begin();
    delay(5000); //muss drinbleiben, damit bot nicht kaputt geht
    Serial.println("Setup done\n");
    recordingTimeInMillis = (unsigned long) recordingTimeInSec * 1000;

    //IP-Adresse (6 für bot mit nummer 6), Gateway, an wen senden (zuhause)
    // WiFi.config(IPAddress(10, 0, 1, 6), IPAddress(10, 0, 2, 1), IPAddress(255, 255, 0, 0));
    // WiFi.config(IPAddress(10, 0, 1, 13), IPAddress(10, 0, 2, 1), IPAddress(255, 255, 0, 0));


    //WLAN, Passwort (zuhause)
    // WiFi.begin("Vodafone-2584", "4EgXXA7R93x9ypCz");
    
    //WLAN, Passwort (UNI)
    // WiFi.begin("DeziWLAN", "Ingwer-Zitrone");

    //WLAN, Passwort (ZackZack)
    // WiFi.begin("ESPRESSOZACKZACK", "WIFIZACKZACK");

    // while (!WiFi.isConnected()){
    //     delay(100);
    //     Serial.println("Connecting to WiFi..");
    // }

    // murmecha::display::clear();
    // murmecha::display::draw_info_screen;
    // murmecha::display::draw_battery_status;
    // murmecha::display::update();

    //4444 port auf meinem Laptop
    //udp.begin(4444);


    startTime = micros(); // Record the start time
    currentTime = startTime; //
    previousTime = currentTime;

    murmecha::motors::set_linear_velocities(0,0);
}

void custom_loop() {

    currentTime = micros();

    // sampleDeltaTime = currentTime - oldTime;
    startDeltaTime = currentTime - startTime;
    murmecha::imu::read_accel_and_gyro();
    auto accel = murmecha::imu::get_acceleration();
    auto gyro = murmecha::imu::get_gyroscope();
    auto entry = IMUData(startDeltaTime, accel, gyro);
    
    // BUFFER FÜLLEN
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
    // sende den Buffer über UDP...

    // IP-Adresse 10, 0, 2 gesetzt, letzte Zahl kann gewählt werden außer 1, 42, 29, 137)
    udp.beginPacket(IPAddress(10, 0, 2, 11), 4444);

    udp.write((uint8_t *) &entry, sizeof(IMUData));
    udp.endPacket();    
}

void custom_loop_position()  {

    currentTime = micros();
    startDeltaTime = currentTime - startTime;

    // Berechnung der Zeitdifferenz seit dem letzten Aufruf
    float sampleDeltaTime = (currentTime - previousTime) / 1000000.0;  // Zeit in Sekunden

    // Speicher die aktuelle Zeit für den nächsten Schleifendurchlauf
    previousTime = currentTime;

    murmecha::imu::read_accel_and_gyro();
    auto accel = murmecha::imu::get_acceleration();
    auto gyro = murmecha::imu::get_gyroscope();
    auto entry = IMUData(startDeltaTime, accel, gyro);

    // vereinfachen der Variablen
    float accelX = entry.accel.x;
    float accelY = entry.accel.y;
    float accelZ = entry.accel.z;
    float gyroX = entry.gyro.x;
    float gyroY = entry.gyro.y;
    float gyroZ = entry.gyro.z;

    
    // Integration der Beschleunigung zur Berechnung der Geschwindigkeit
    velocityX += accelX * sampleDeltaTime;
    velocityY += accelY * sampleDeltaTime;
    velocityZ += accelZ * sampleDeltaTime;

    // Integration der Geschwindigkeit zur Berechnung der Distanz/Position
    posX += velocityX * sampleDeltaTime;
    posY += velocityY * sampleDeltaTime;
    posZ += velocityZ * sampleDeltaTime;

    uint64_t time = micros();
    Serial.printf("%llu %f %f %f\n", time, posX, posY, posZ);

    
    if (loopCounter == 1000) {
        // Positionsdaten als JSON-String vorbereiten
        //String data = "{ \"x\": " + String(posX) + ", \"y\": " + String(posY) + ", \"z\": " + String(posZ) + " }";

        // Positionsdaten über WiFi senden
        // IP-Adresse 10, 0, 2 gesetzt, letzte Zahl kann gewählt werden außer 1, 42, 29, 137)
        //udp.beginPacket(IPAddress(10, 0, 2, 11), 4444);
        //udp.write((uint8_t *) data.c_str(), data.length());
        //udp.write((uint8_t *) &entry, sizeof(IMUData));

        //udp.endPacket(); 

        //Serial.println(data);

        loopCounter = 0;
    }

    loopCounter += 1;
}