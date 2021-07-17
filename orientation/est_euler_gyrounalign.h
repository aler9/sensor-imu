
#pragma once

#include "../imu.h"

#include "est.h"
#include "vector.h"

typedef void est_euler_gyrounalignt;

error *est_euler_gyrounalign_init(est_euler_gyrounalignt **pobj,
                                  const vector *gyro_bias);
void est_euler_gyrounalign_do(est_euler_gyrounalignt *obj, const double *gyro,
                              double dt, estimator_output *eo);
