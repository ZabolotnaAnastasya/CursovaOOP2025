//
// Created by Anastasiia Zabolotna on 15.11.2025.
//

#ifndef VECTOR_H
#define VECTOR_H

#include <Eigen/Core>

class Vector {
private:
    Eigen::Vector3f vec;

public:
    Vector(float x = 0.0f, float y = 0.0f, float z = 0.0f);
    Vector(const Eigen::Vector3f& v);

    float getX() const { return vec.x(); }
    float getY() const { return vec.y(); }
    float getZ() const { return vec.z(); }

    void setV(float x, float y, float z);

    Vector operator+(const Vector& v) const;
    Vector operator-(const Vector& v) const;
    Vector operator*(float scalar) const;

    Eigen::Vector3f toEigen() const;
};

#endif //VECTOR_H