#include "KalmanFilter.h"
#include <cmath>

KalmanFilter::KalmanFilter(float process_noise, float measurement_noise) {
    // п1 ініціалізація
    n_x = 3;
    lambda = 3 - n_x;

    x.setZero();
    P = Eigen::Matrix3d::Identity(); // одинична матриця

    // матриці шуму (діагональні, вважаєм незалежність шумів сенсорів)
    Q = Eigen::Matrix3d::Identity() * (process_noise * process_noise);
    R = Eigen::Matrix3d::Identity() * (measurement_noise * measurement_noise);

    // Ваги для сигма-точок (2*n + 1 = 7 точок)
    weights = Eigen::VectorXd(2 * n_x + 1);
    weights(0) = lambda / (lambda + n_x);
    for (int i = 1; i < 2 * n_x + 1; ++i) {
        weights(i) = 0.5 / (lambda + n_x);
    }

    Xsig_pred = Eigen::MatrixXd(n_x, 2 * n_x + 1);
}

Vector KalmanFilter::update(const Vector& measurement) {
    Eigen::Vector3d z_in;
    z_in << measurement.getX(), measurement.getY(), measurement.getZ();

    // п2 генерація сигма-точок
    Eigen::MatrixXd Xsig = Eigen::MatrixXd(n_x, 2 * n_x + 1);

    // корінь з матриці P
    // P = L * L^T
    Eigen::MatrixXd L = P.llt().matrixL();

    Xsig.col(0) = x;
    double sqrt_lambda_n = std::sqrt(lambda + n_x);

    for (int i = 0; i < n_x; ++i) {
        Xsig.col(i + 1)       = x + sqrt_lambda_n * L.col(i);
        Xsig.col(i + 1 + n_x) = x - sqrt_lambda_n * L.col(i);
    }

    // п3 передбачення
    Xsig_pred = Xsig;

    // прогноз середнього
    Eigen::Vector3d x_pred = Eigen::Vector3d::Zero();
    for (int i = 0; i < 2 * n_x + 1; ++i) {
        x_pred += weights(i) * Xsig_pred.col(i);
    }

    // прогноз коваріації
    Eigen::Matrix3d P_pred = Eigen::Matrix3d::Zero();
    for (int i = 0; i < 2 * n_x + 1; ++i) {
        Eigen::Vector3d x_diff = Xsig_pred.col(i) - x_pred;
        P_pred += weights(i) * x_diff * x_diff.transpose();
    }
    P_pred += Q;

    // п4 оновлення
    //int n_z = 3;
    Eigen::MatrixXd Zsig = Xsig_pred; // Сигма-точки у просторі вимірювань
    Eigen::Vector3d z_pred = x_pred;  // Передбачене вимірювання

    // коваріація вимірювання S
    Eigen::Matrix3d S = Eigen::Matrix3d::Zero();
    for (int i = 0; i < 2 * n_x + 1; ++i) {
        Eigen::Vector3d z_diff = Zsig.col(i) - z_pred;
        S += weights(i) * z_diff * z_diff.transpose();
    }
    S += R;

    // крос-коваріація Tc
    Eigen::Matrix3d Tc = Eigen::Matrix3d::Zero();
    for (int i = 0; i < 2 * n_x + 1; ++i) {
        Eigen::Vector3d z_diff = Zsig.col(i) - z_pred;
        Eigen::Vector3d x_diff = Xsig_pred.col(i) - x_pred;
        Tc += weights(i) * x_diff * z_diff.transpose();
    }

    // Kalman Gain
    Eigen::Matrix3d K = Tc * S.inverse();

    // оновлення стану
    Eigen::Vector3d z_diff = z_in - z_pred;
    x = x_pred + K * z_diff;
    P = P_pred - K * S * K.transpose();

    return Vector((float)x(0), (float)x(1), (float)x(2));
}