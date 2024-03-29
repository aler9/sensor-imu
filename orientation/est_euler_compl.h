
#pragma once

#include "../imu.h"

#include "est.h"
#include "vector.h"

#define EST_EULER_COMPL_DEFAULT_ALPHA 0.1f

typedef void est_euler_complt;

error *est_euler_compl_init(est_euler_complt **pobj, const matrix *align_dcm,
                            const vector *gyro_bias, double alpha);
void est_euler_compl_do(est_euler_complt *obj, const double *acc,
                        const double *gyro, double dt, estimator_output *eo);
