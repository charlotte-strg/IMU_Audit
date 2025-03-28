#ifndef QUATERNION_H
#define QUATERNION_H

#include <cmath>

struct Quaternion {
    float w, x, y, z;

    static Quaternion identity() {
        return Quaternion{1.0, 0.0, 0.0, 0.0};
    }
};

Quaternion inverseQuaternion(const Quaternion& q);
Quaternion multiplyQuaternions(const Quaternion& q1, const Quaternion& q2);
double calculateRotationAngle(const Quaternion& q_initial, const Quaternion& q_current);

#endif
