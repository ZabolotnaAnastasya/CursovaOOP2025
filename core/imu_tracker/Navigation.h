//
// Created by Anastasiia Zabolotna on 15.11.2025.
//

#ifndef NAVIGATION_H
#define NAVIGATION_H

#include <vector>
#include <deque>
#include "Vector.h"
#include "Quaternion.h"
#include "KalmanFilter.h"

class Navigation {
public:
    Navigation(int window_size, float acc_thresh,
               float acc_p_noise, float acc_m_noise,
               float gyro_p_noise, float gyro_m_noise,
               float v_x0, float v_y0, float v_z0);
    void process_reading(float ax, float ay, float az, float gx, float gy, float gz, float dt);
    std::vector<Vector> get_trajectory() const;

private:
    KalmanFilter kf_accel;
    KalmanFilter kf_gyro;

    Quaternion orientation;
    Vector velocity;
    Vector position;

    Vector prev_accel_global;
    Vector prev_velocity;
    Vector prev_gyro;
    bool first_run = true;

    std::deque<Vector> accel_history;
    int zupt_window_size;
    float acceleration_threshold;
    std::vector<Vector> trajectory;

    bool is_stationary();
};

#endif // NAVIGATION_H