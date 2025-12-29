//
// Created by Anastasiia Zabolotna on 15.11.2025.
//

#include "Vector.h"

Vector::Vector(float x, float y, float z) : vec(x, y, z) {}

// Конструктор для внутр
Vector::Vector(const Eigen::Vector3f& v) : vec(v) {}

void Vector::setV(float x, float y, float z) {
    vec.x() = x;
    vec.y() = y;
    vec.z() = z;
}

Vector Vector::operator+(const Vector& other) const {
    return Vector(this->vec + other.vec);
}

Vector Vector::operator-(const Vector& other) const {
    return Vector(this->vec - other.vec);
}

Vector Vector::operator*(float scalar) const {
    return Vector(this->vec * scalar);
}

Eigen::Vector3f Vector::toEigen() const {
    return vec;
}
