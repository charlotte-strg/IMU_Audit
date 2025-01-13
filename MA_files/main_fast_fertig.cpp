#include <Arduino.h>

#include "MadgwickAHRS.h"

#include <murmecha.hpp>
#include <WiFi.h>

WiFiUDP udp;

struct __attribute__((__packed__)) gps_data_t {
  uint8_t robot_id;
  float x;
  float y;
  float phi;
};

uint64_t delta_t = 0;

murmecha::math::Vector3 computed_drifts;

void madgwick_stuff(void *) {
  while (true) {
    uint64_t bt = micros();
    murmecha::imu::read_accel_and_gyro();
    const auto & acc = murmecha::imu::get_acceleration();
    const auto & gyro = murmecha::imu::get_gyroscope();

    murmecha::math::Vector3 computed_gyro = gyro - computed_drifts;
    
    MadgwickAHRSupdateIMU(computed_gyro.x, computed_gyro.y, computed_gyro.z, acc.x, acc.y, acc.z);
    delay(1);
    delta_t = micros() - bt;
  }
}

xTaskHandle bgTask;
const uint32_t sample_count=1000;
murmecha::math::Vector3 samples[sample_count];

murmecha::math::Vector3 measure_gyro_drift() {

  for (uint32_t i = 0; i < sample_count; ++i) {
    murmecha::imu::read_accel_and_gyro();
    samples[i] = murmecha::imu::get_gyroscope();
    delay(1);
  }

  murmecha::math::Vector3 mean;
  
  for (uint32_t i = 0; i < sample_count; ++i) {
    mean = mean + samples[i];
  }

  return {mean.x / sample_count, mean.y / sample_count, mean.z / sample_count};
  
}

void setup() {

  Serial.begin(115200);
  murmecha::begin();

  murmecha::imu::set_accel_data_rate(murmecha::imu::DATA_RATE_1_6kHz);
  murmecha::imu::set_gyro_data_rate(murmecha::imu::DATA_RATE_1_6kHz);
  
  murmecha::display::invert();
  murmecha::display::draw_tracking_code(42);
  murmecha::display::draw_info_screen(42);

  murmecha::motors::beep(1600, 1000);
  
  murmecha::rgb::set_led_color(0, 64, 0, 0);
  WiFi.config(IPAddress(10, 0, 1, 42), IPAddress(10, 0, 2, 1), IPAddress(255, 255, 0, 0));
  WiFi.begin("DeziWLAN", "Ingwer-Zitrone");
  float t = 0.0f;
  while (!WiFi.isConnected()) {
    //Serial.println("Connecting...");
    murmecha::rgb::set_led_color(0, 32 * sin(t) + 32, 0, 0);
    delay(10);
    t += 0.03;
  }

  murmecha::rgb::set_led_color(0, 0, 0, 0);
  
  //Serial.printf("Starting measurements\n");
  computed_drifts = measure_gyro_drift();
  //Serial.printf("Beta should be around: %f\n", computed_beta);

  Serial.printf("Drift: (%f %f %f)\n", computed_drifts.x, computed_drifts.y, computed_drifts.z);
  
  /*for (uint32_t i = 0; i < sample_count; ++i) {
    Serial.printf("%f %f %f\n", samples[i].x, samples[i].y, samples[i].z);
    }*/
  
  xTaskCreatePinnedToCore(madgwick_stuff, "imu", 4096, nullptr, 1, &bgTask, 1);

  //udp.begin(4201);
  //udp.begin(4442);
  
  delay(5000);

  delay(500);
  murmecha::motors::beep(1600, 500);
  delay(500);

  #if MURMECHA_HARDWARE_REV == 1 && UNDEF
  murmecha::uv::set(1);
  for (size_t i = 0; i < 300; ++i) {
    float l = 25 + 10 * (std::sin(0.01 * i * M_PI * 2) / 2 + 0.5);
    float r = 25 + 10 * (std::sin(0.01 * i * M_PI * 2 + M_PI) / 2 + 0.5);
    murmecha::motors::set_linear_velocities(l, r);
    murmecha::display::clear();
    murmecha::display::draw_info_screen(42);
    murmecha::display::draw_tracking_code(42);
    //murmecha::display::draw_string(0, 16, buffer);
    murmecha::display::update();
    delay(100);
  }
  murmecha::uv::set(0);
  #endif
  
  //pinMode(12, OUTPUT);
  //digitalWrite(12, 1);
  
  // murmecha::uv::set(1.0f);
  // murmecha::motors::set_linear_velocities(60.0f, 60.0f);
  //murmecha::phototransistors::set_gain(murmecha::phototransistors::GAIN_2x);

  //murmecha::motors::set_linear_velocities(30, 30);

  /*gps_data_t data;
  while (data.robot_id != 19) {
    while (!udp.parsePacket()) {
      Serial.printf("Waiting for packet\n");
      delay(100);
    }
    udp.read((char*) &data, sizeof(data));
    }*/

  murmecha::motors::beep(1200, 1000);

  murmecha::uv::set(0.5);
  
}

uint32_t count = 0;
bool charged = false;

float R_perp = 37.0f;

void drive_segment(float length, float vm) {

  float time = length / vm;
  murmecha::motors::set_linear_velocities(vm, vm);
  delay(time*1000);
  murmecha::motors::set_linear_velocities(0, 0);

}

void drive_circle(float radius, float vm) {

  float R_l = radius - R_perp;
  float R_r = radius + R_perp;

  float v_r = 2 * vm / (1 + (R_l / R_r));
  float v_l = v_r * (R_l / R_r);

  murmecha::motors::set_linear_velocities(v_l, v_r);

}

void drive_curve(float radius, float angle, float vm) {

  float dist = radius * angle;
  float time = dist / vm;

  drive_circle(radius, vm);

  float measured_angle = 0;
  q0 = 1;
  q1 = 0;
  q2 = 0;
  q3 = 0;
  
  while (measured_angle < angle) {
    measured_angle = atan2f(sqrtf(q1*q1 + q2*q2 + q3*q3), q0);
    delay(1);
  }
  
  //delay(time * 1000);
  murmecha::motors::set_linear_velocities(0, 0);

}

void loop() {

  drive_segment(250.0f, 30.0f);
  drive_curve(100.0f, M_PI/2, 30.0f);

  udp.beginPacket(IPAddress(10, 0, 2, 137), 4444);
  udp.write((uint8_t *) &q0, sizeof(q0));
  udp.write((uint8_t *) &q1, sizeof(q1));
  udp.write((uint8_t *) &q2, sizeof(q2));
  udp.write((uint8_t *) &q3, sizeof(q3));
  udp.write((uint8_t *) &delta_t, sizeof(delta_t));
  udp.endPacket();

  delay(100);
  
}

#ifdef UNDEF
void loop() {

  #if MURMECHA_HARDWARE_REV == 2
  int32_t P = murmecha::battery::get_consumed_power();
  #else
  int32_t P = -1;
  #endif
  char buffer[64];  
  sprintf(buffer, "P = %d mW", P);
  
  /*while (size_t size = udp.parsePacket()) {
    Serial.printf("has some data available: %d\n", size);
    gps_data_t data;
    udp.read((uint8_t *) &data, sizeof(data));
    Serial.printf("Got message for %d %f %f %f\n", data.robot_id, data.x, data.y, data.phi);
    }*/
  
  if (P <= 0) {
    murmecha::display::clear();
    murmecha::display::draw_info_screen(42);
    murmecha::display::draw_tracking_code(42);
    murmecha::display::update();
    murmecha::uv::set(0.0);
    murmecha::motors::set_linear_velocities(0, 0);
    charged = true;      
    return;
  }

  if (charged) {

    delay(5000);
    murmecha::uv::set(1.0);
    for (size_t i = 0; i < 300; ++i) {
      float l = 30 * (std::sin(0.01 * i * M_PI * 2) / 2 + 0.5);
      float r = 30 * (std::cos(0.01 * i * M_PI * 2) / 2 + 0.5);
      murmecha::motors::set_linear_velocities(l, r);
      murmecha::display::clear();
      murmecha::display::draw_info_screen(42);
      murmecha::display::draw_tracking_code(42);
      //murmecha::display::draw_string(0, 16, buffer);
      murmecha::display::update();
      delay(100);
    }
    murmecha::uv::set(0.0);
    
    charged = false;
    return;
  }
  
  auto [left, center, right] = murmecha::phototransistors::get_values();

  /*udp.beginPacket(IPAddress(10, 0, 2, 137), 4444);
  udp.write((uint8_t*) &left, sizeof(left));
  udp.write((uint8_t*) &center, sizeof(center));
  udp.write((uint8_t*) &right, sizeof(right));
  udp.write((uint8_t*) &P, sizeof(P));
  udp.endPacket();*/
  
  murmecha::display::clear();
  murmecha::display::draw_info_screen(42);
  murmecha::display::draw_tracking_code(42);
  //murmecha::display::draw_string(0, 16, buffer);
  murmecha::display::update();
  count++;
  //murmecha::erase::set((float)(count % 256) / 255.0);
  //murmecha::erase::set_absolute(1);
  murmecha::uv::set(0.0);
  delay(100);

}
// 8636999
#endif
