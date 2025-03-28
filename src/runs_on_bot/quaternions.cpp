#include <cmath>

// Aufbau Quaternion
struct Quaternion {
    float w, x, y, z;

    // einheitsquaternion
    static Quaternion identity() {
        return Quaternion{1.0, 0.0, 0.0, 0.0};
    }
};

// Berechnung der Inversen
Quaternion inverseQuaternion(const Quaternion& q) {
    return {q.w, -q.x, -q.y, -q.z};
}

// Multiplikation von 2 Quaternionen
Quaternion multiplyQuaternions(const Quaternion& q1, const Quaternion& q2) {
    Quaternion result;
    result.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
    result.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
    result.y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x;
    result.z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w;
    return result;
}

template <typename T> int sign(T val) {
    return (T(0) < val) - (val < T(0));
}

// Drehwinkel zwischen 2 Positionen (quaternions) errechnen in Bogenmaß
double calculateRotationAngle(const Quaternion& q_initial, const Quaternion& q_current) {
    // Berechne Inverse von q_initial
    Quaternion q_initial_inv = inverseQuaternion(q_initial);

    // Berechne relative Quaternion (multipliziere aktuelle Position mit Inverse der initialen Quaternion)
    Quaternion q_relative = multiplyQuaternions(q_current, q_initial_inv);

    auto length = sqrtf(q_relative.x*q_relative.x + q_relative.y*q_relative.y + q_relative.z*q_relative.z);

    // Berechne Drehwinkel in Bogenmaß
    double angle = 2.0 * std::acos(q_relative.w);

    return sign(q_relative.z / length) * angle;;
}