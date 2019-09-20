
#pragma once

typedef void est_euler_gyrot;

error* est_euler_gyro_init(est_euler_gyrot** pobj, imu_autot* imu);
void est_euler_gyro_do(est_euler_gyrot* obj,
    const double* gyro, double dt, estimator_output* eo);
