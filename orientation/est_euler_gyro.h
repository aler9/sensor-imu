
#pragma once

#include "../imu.h"

#include "est.h"
#include "vector.h"

typedef void est_euler_gyrot;

error *est_euler_gyro_init(est_euler_gyrot **pobj, const matrix *align_dcm,
                           const vector *gyro_bias);
void est_euler_gyro_do(est_euler_gyrot *obj, const double *gyro, double dt,
                       estimator_output *eo);
