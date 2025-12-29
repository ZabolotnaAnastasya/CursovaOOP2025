//
// Created by Anastasiia Zabolotna on 15.11.2025.
//
#ifndef QUATERNION_H
#define QUATERNION_H

#include <Eigen/Geometry>
#include "Vector.h"

class Quaternion {
private:
    Eigen::Quaternionf q;

public:
    Quaternion(float w, float x, float y, float z);
    Quaternion(const Eigen::Quaternionf& eigenQ);

    void updateFromGyro(const Vector& curr_gyro, const Vector& prev_gyro, float dt);
    void updateFromGyro(const Vector& gyro, float dt);

    void normalize();
    Quaternion q_1() const;
    Vector rotate(const Vector& v) const;

    Quaternion operator*(const Quaternion& q) const;
    Quaternion operator+(const Quaternion& q) const;

    float w() const { return q.w(); }
    float x() const { return q.x(); }
    float y() const { return q.y(); }
    float z() const { return q.z(); }
};

#endif //QUATERNION_H