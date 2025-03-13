#ifndef IMU_DATA_H
#define IMU_DATA_H

#include <imu.hpp>

struct IMUData {
    uint64_t time_micros;
    murmecha::math::Vector3 accel;
    murmecha::math::Vector3 gyro;

    IMUData()
        : time_micros(), accel{0, 0, 0}, gyro{0, 0, 0} {}

    IMUData(uint64_t t_mic, murmecha::math::Vector3 a, murmecha::math::Vector3 g)
        // : id(i), isSet(true), accel(a), gyro(g) {}
        : time_micros(t_mic), accel(a), gyro(g) {}

};

#endif