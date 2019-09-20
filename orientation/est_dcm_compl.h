
#pragma once

typedef void est_dcm_complt;

error* est_dcm_compl_init(est_dcm_complt** pobj, imu_autot* imu);
void est_dcm_compl_do(est_dcm_complt* obj, const double* acc,
    const double* gyro, double dt, estimator_output* eo);
