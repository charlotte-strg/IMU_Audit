

#include <cmath>

struct Quaternion {
    double w, x, y, z;

    static Quaternion identity() {
        return Quaternion{1.0, 0.0, 0.0, 0.0};
    }
};

Quaternion inverseQuaternion(const Quaternion& q) {
    return {q.w, -q.x, -q.y, -q.z};
}

Quaternion multiplyQuaternions(const Quaternion& q1, const Quaternion& q2) {
    Quaternion result;
    result.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
    result.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
    result.y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x;
    result.z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w;
    return result;
}

// Funktion zur Berechnung des Drehwinkels zwischen zwei Quaternions in Bogenmaß
double calculateRotationAngle(const Quaternion& q_initial, const Quaternion& q_current) {
    // Schritt 1: Berechnung des inversen Quaternions von q_initial
    Quaternion q_initial_inv = inverseQuaternion(q_initial);

    // Schritt 2: Berechnung des relativen Quaternions
    Quaternion q_relative = multiplyQuaternions(q_current, q_initial_inv);

    // Schritt 3: Berechnung des Drehwinkels in Bogenmaß
    double angle = 2.0 * std::acos(q_relative.w);

    // Rückgabe des Winkels
    return angle;
}