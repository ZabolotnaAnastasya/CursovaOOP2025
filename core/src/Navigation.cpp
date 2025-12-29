//
// Created by Anastasiia Zabolotna on 15.11.2025.
//
#include "Navigation.h"
#include <cmath>
#include <iostream>

Navigation::Navigation(int window_size, float acc_thresh,
                       float acc_p_noise, float acc_m_noise,
                       float gyro_p_noise, float gyro_m_noise):
    kf_accel(acc_p_noise, acc_m_noise),
    kf_gyro(gyro_p_noise, gyro_m_noise),
    orientation(1.0f, 0.0f, 0.0f, 0.0f),
    zupt_window_size(window_size),
    acceleration_threshold(acc_thresh)
{
    velocity = Vector(0, 0, 0);
    position = Vector(0, 0, 0);

    prev_accel_global = Vector(0, 0, 0);
    prev_velocity = Vector(0, 0, 0);
    prev_gyro = Vector(0, 0, 0);
}

// ZUPT
bool Navigation::is_stationary() {
    if (accel_history.size() < zupt_window_size) return false;

    // середнє
    float sum = 0.0f;
    for (const auto& v : accel_history) {
        float mag = std::sqrt(v.getX()*v.getX() + v.getY()*v.getY() + v.getZ()*v.getZ());
        sum += mag;
    }
    float mean = sum / accel_history.size();

    // дисперсія
    float variance = 0.0f;
    for (const auto& v : accel_history) {
        float mag = std::sqrt(v.getX()*v.getX() + v.getY()*v.getY() + v.getZ()*v.getZ());
        float diff = mag - mean;
        variance += diff * diff;
    }
    variance /= accel_history.size();

    return variance < acceleration_threshold;
}


void Navigation::process_reading(float ax, float ay, float az, float gx, float gy, float gz, float dt) {
    Vector raw_accel(ax, ay, az);
    Vector raw_gyro(gx, gy, gz);

    Vector accel_vec = kf_accel.update(raw_accel);
    Vector gyro_vec  = kf_gyro.update(raw_gyro);

    // історія для ZUPT
    accel_history.push_back(accel_vec);
    if (accel_history.size() > zupt_window_size) {
        accel_history.pop_front();
    }

    if (first_run) {
        prev_gyro = gyro_vec;

        orientation.updateFromGyro(gyro_vec, dt);
        Vector world_accel = orientation.rotate(accel_vec);

        prev_accel_global = world_accel;
        prev_velocity = Vector(0, 0, 0);

        first_run = false;
        return;
    }

    // оновлення орієнтації
    orientation.updateFromGyro(gyro_vec, prev_gyro, dt);

    // у світ коорд
    Vector world_accel = orientation.rotate(accel_vec);

    // інтеграл
    if (is_stationary()) {
        velocity = Vector(0, 0, 0);
        // Коли стоїмо, прискорення теж обнуляємо, щоб не накопичувати дрейф
        prev_accel_global = Vector(0,0,0);
    } else {
        Vector delta_v = (world_accel + prev_accel_global) * (0.5f * dt);
        velocity = velocity + delta_v;

        Vector delta_p = (velocity + prev_velocity) * (0.5f * dt);
        position = position + delta_p;

        prev_accel_global = world_accel;
    }

    // оновлення історії
    prev_gyro = gyro_vec;
    prev_velocity = velocity;

    trajectory.push_back(position);
}

std::vector<Vector> Navigation::get_trajectory() const {
    return trajectory;
}