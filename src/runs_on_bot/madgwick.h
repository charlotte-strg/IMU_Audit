#ifndef FILTER_HPP
#define FILTER_HPP

#include <cmath>
#include <array>
#include "quaternions.h"

// wendet den Madgwick-Filter auf die Sensordaten an, gibt Quaternion zurück
// q: aktuelle Quaternion
// w_x, w_y, w_z: Winkelgeschwindigkeiten in rad/s
// a_x, a_y, a_z: Beschleunigungen in g
Quaternion madgwick_filter(
    const Quaternion& q,
    float w_x, float w_y, float w_z,
    float a_x, float a_y, float a_z
);

#endif // FILTER_HPP