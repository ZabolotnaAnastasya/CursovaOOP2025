//
// Created by Anastasiia Zabolotna on 15.11.2025.
//
#include "Quaternion.h"


Quaternion::Quaternion(float w, float x, float y, float z) {
    q = Eigen::Quaternionf(w, x, y, z);
    q.normalize();
}

Quaternion::Quaternion(const Eigen::Quaternionf& eigenQ) : q(eigenQ) {
    q.normalize();
}


void Quaternion::updateFromGyro(const Vector& curr_gyro, const Vector& prev_gyro, float dt){
    Vector w_avg = (curr_gyro + prev_gyro) * 0.5f;

    // кутов шв в айген     |w| = sqrt(wx^2 + wy^2 + wz^2)
    Eigen::Vector3f w = w_avg.toEigen();
    float angle = w.norm() * dt;    // |w| * dt

    if (angle > 1e-6f) { // ділення на нуль
        Eigen::Vector3f axis = w.normalized();
        // кватерніон повороту  dq =[cos(a/2), x*sin(a/2), y*sin(a/2), z*sin(a/2)]
        Eigen::Quaternionf delta_q(Eigen::AngleAxisf(angle, axis));

        // q_new = q_old * delta_q = *this + (*this * dq);
        q = q * delta_q;
        q.normalize();
    }
}

void Quaternion::updateFromGyro(const Vector& curr_gyro, float dt) {
    updateFromGyro(curr_gyro, curr_gyro, dt);
}

void Quaternion::normalize() {
    q.normalize();
}

Quaternion Quaternion::q_1() const {
    return Quaternion(q.conjugate());
}

Vector Quaternion::rotate(const Vector& v) const {
    // оператор * робить поворот вектора автоматично
    return Vector(q * v.toEigen());
}

Quaternion Quaternion::operator*(const Quaternion& other) const {
    return Quaternion(this->q * other.q);
}

Quaternion Quaternion::operator+(const Quaternion& other) const {
    Eigen::Quaternionf sum;
    sum.coeffs() = this->q.coeffs() + other.q.coeffs();
    return Quaternion(sum);
}