#include <Arduino.h>
#include <WiFi.h>
#include <murmecha.hpp>
#include "imu_madgwick_integration.h"

extern murmecha::math::Vector3 computed_drifts;
murmecha::math::Vector3 measure_gyro_drift();
float pid_control(float target_rad, float current_rad);
extern Quaternion q_initial_pid;

float target_angle = 0;
std::tuple<float, float> pid_to_wheel_speeds(float pid_output, float base_speed);

TaskHandle_t pid_task_handle;
TaskHandle_t madgwick_task_handle;

extern WiFiUDP udp;

void update_madgwick(void * args) {

while (true){
    update_orientation_madgwick(computed_drifts);
    delay(1);
}
}

void update_pid(void * args) {

while (true) {
    float angle = calculateRotationAngle(q_initial_pid, orientation_madgwick);
    auto pid_out = pid_control(target_angle, angle);
    auto [l, r] = pid_to_wheel_speeds(pid_out, 60.0f);
    murmecha::motors::set_linear_velocities(l, r);
    Serial.printf("%f\n", pid_out);

    // Senden an Laptop mit Format f-f-f-f-f
    // udp.beginPacket(IPAddress(10, 0, 2, 137), 4444);
    // udp.write((uint8_t *) &angle, sizeof(angle));
    // udp.write((uint8_t *) &target_angle, sizeof(target_angle));
    // udp.write((uint8_t *) &pid_out, sizeof(pid_out));
    // udp.write((uint8_t *) &l, sizeof(l));
    // udp.write((uint8_t *) &r, sizeof(r));
    // udp.endPacket();
    
    delay(1);
}

}

struct accel_gyro_data_t {
murmecha::math::Vector3 acc;
murmecha::math::Vector3 gyro;
};

const uint32_t imu_sample_count = 2048;
accel_gyro_data_t imu_samples[imu_sample_count];

murmecha::math::Vector3 get_drift() {

for (uint32_t i = 0; i < imu_sample_count; ++i) {
    murmecha::imu::read_accel_and_gyro();
    imu_samples[i].acc  = murmecha::imu::get_acceleration();
    imu_samples[i].gyro = murmecha::imu::get_gyroscope();
    delay(1);
}

murmecha::math::Vector3 mean;
Serial.printf("ax ay az gx gy gz\n");
for (uint32_t i = 0; i < imu_sample_count; ++i) {
    mean = mean + imu_samples[i].gyro;
    Serial.printf("%f %f %f %f %f %f\n", imu_samples[i].acc.x, imu_samples[i].acc.y, imu_samples[i].acc.z, imu_samples[i].gyro.x, imu_samples[i].gyro.y, imu_samples[i].gyro.z);
}

return {mean.x / imu_sample_count, mean.y / imu_sample_count, mean.z / imu_sample_count};

}

void setup_square_pid() {

Serial.begin(115200);
murmecha::begin();

murmecha::display::invert();
murmecha::display::clear();
murmecha::display::draw_tracking_code(16);
murmecha::display::draw_info_screen(16);
murmecha::display::update();
murmecha::imu::set_accel_data_rate(murmecha::imu::DATA_RATE_1_6kHz);
murmecha::imu::set_gyro_data_rate(murmecha::imu::DATA_RATE_1_6kHz);

WiFi.config(IPAddress(10, 0, 1, 22), IPAddress(10, 0, 2, 1), IPAddress(255, 255, 0, 0));

//Insert your WiFi credentials here:
String network;
String pass_key;

WiFi.begin(network, pass_key);
while (!WiFi.isConnected()){
    delay(100);
    Serial.println("Connecting to WiFi..");
}
// udp.begin(4444);

delay(2000);
murmecha::motors::beep(1200, 500);

//computed_drifts = measure_gyro_drift();
computed_drifts = get_drift();

xTaskCreatePinnedToCore(update_pid, "pid_task", 4096, nullptr, 2, &pid_task_handle, 0);
xTaskCreatePinnedToCore(update_madgwick, "AHRS", 4096, nullptr, 1, &madgwick_task_handle, 1);

}

void loop_square_pid() {
 
   //murmecha::motors::set_linear_velocities(60, 60);
   //delay(1000);
   
   
   const int step_count = 100;
   
   delay(3000);
   for (int i = 0; i < step_count; ++i) {
     target_angle += (M_PI / 2) / (float) step_count;
     target_angle = atan2f(sinf(target_angle), cosf(target_angle));
     delay(3000 / step_count);
   }
   delay(3000);
 }