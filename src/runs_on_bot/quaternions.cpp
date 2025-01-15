#include <cmath>

// aufbau quaternionen
struct Quaternion {
    float w, x, y, z;

    // einheitsquaternion
    static Quaternion identity() {
        return Quaternion{1.0, 0.0, 0.0, 0.0};
    }
};

// berechnung des inversen quaternions
Quaternion inverseQuaternion(const Quaternion& q) {
    return {q.w, -q.x, -q.y, -q.z};
}

// multiplikation zweier quaternions
Quaternion multiplyQuaternions(const Quaternion& q1, const Quaternion& q2) {
    Quaternion result;
    result.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
    result.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
    result.y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x;
    result.z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w;
    return result;
}

// drehwinkel zwischen 2 positionen (quaternions) errechnen in bogenmaß
double calculateRotationAngle(const Quaternion& q_initial, const Quaternion& q_current) {
    // berechne inverses quaternion von q_initial
    Quaternion q_initial_inv = inverseQuaternion(q_initial);

    // berechne relatives quaternion (multipliziere aktuelle position mit inversen initialen quaternion)
    Quaternion q_relative = multiplyQuaternions(q_current, q_initial_inv);

    // berechne drehwinkels in bogenmaß
    double angle = 2.0 * std::acos(q_relative.w);

    return angle;
}