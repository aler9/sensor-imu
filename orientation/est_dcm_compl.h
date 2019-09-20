
#pragma once

typedef void est_dcm_complt;

error* est_dcm_compl_init(est_dcm_complt** pobj, const matrix* align_dcm,
    const vector* gyro_bias);
void est_dcm_compl_do(est_dcm_complt* obj, const double* acc,
    const double* gyro, double dt, estimator_output* eo);
