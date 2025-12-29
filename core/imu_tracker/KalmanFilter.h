#ifndef KALMANFILTER_H
#define KALMANFILTER_H

#include <Eigen/Dense>
#include "Vector.h" // Підключаємо твій клас Vector

class KalmanFilter {
public:
    KalmanFilter(float process_noise, float measurement_noise);

    Vector update(const Vector& measurement);

private:
    // стан системи ( вектор 3x1)
    Eigen::Vector3d x;

    // матриця коваріації 3x3
    Eigen::Matrix3d P;

    // шуми (матриці 3x3)
    Eigen::Matrix3d Q;
    Eigen::Matrix3d R;
    // Q велике, R мале -> сильна довіра
    // Q мале, R велике -> слаба довіра

    // параметри UKF
    int n_x;       // розмірність (3)
    double lambda; // параметр масштабування

    // ваги
    Eigen::VectorXd weights;

    // сигма-точки (матриця 3 x 7)
    Eigen::MatrixXd Xsig_pred;
};

#endif // KALMANFILTER_H