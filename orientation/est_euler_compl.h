
#pragma once

typedef void est_euler_complt;

error* est_euler_compl_init(est_euler_complt** pobj, imu_autot* imu);
void est_euler_compl_do(est_euler_complt* obj, const double* acc,
    const double* gyro, double dt, estimator_output* eo);
