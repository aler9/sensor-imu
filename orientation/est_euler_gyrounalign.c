
#include <math.h>
#include <stdlib.h>

#include "est_euler_gyrounalign.h"

typedef struct {
    const vector *gyro_bias;
    double prev_roll;
    double prev_pitch;
    double prev_yaw;
} _objt;

error *est_euler_gyrounalign_init(est_euler_gyrounalignt **pobj,
                                  const vector *gyro_bias) {
    _objt *_obj = malloc(sizeof(_objt));

    _obj->gyro_bias = gyro_bias;
    _obj->prev_roll = 0;
    _obj->prev_pitch = 0;
    _obj->prev_yaw = 0;

    *pobj = _obj;
    return NULL;
}

void est_euler_gyrounalign_do(est_euler_gyrounalignt *obj, const double *gyro,
                              double dt, estimator_output *eo) {
    _objt *_obj = (_objt *)obj;

    vector tuned_gyro;
    vector_diff((const vector *)gyro, _obj->gyro_bias, &tuned_gyro);

    _obj->prev_roll = _obj->prev_roll + tuned_gyro.x * dt;
    _obj->prev_pitch = _obj->prev_pitch + tuned_gyro.y * dt;
    _obj->prev_yaw = _obj->prev_yaw + tuned_gyro.z * dt;

    eo->roll = _obj->prev_roll;
    eo->pitch = _obj->prev_pitch;
    // eo->yaw = _obj->prev_yaw;
    // do not use yaw, such that the result can be compared with other
    // estimators
    eo->yaw = 0;
}
