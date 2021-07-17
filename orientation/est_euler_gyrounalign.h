
#pragma once

typedef void est_euler_gyrounalignt;

error *est_euler_gyrounalign_init(est_euler_gyrounalignt **pobj,
                                  const vector *gyro_bias);
void est_euler_gyrounalign_do(est_euler_gyrounalignt *obj, const double *gyro,
                              double dt, estimator_output *eo);
