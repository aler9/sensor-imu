
#include <math.h>
#include <stdlib.h>

#include "../imu.h"

#include "est.h"
#include "vector.h"

#include "est_euler_gyro.h"

typedef struct {
    const matrix *align_dcm;
    const vector *gyro_bias;
    double prev_roll;
    double prev_pitch;
    double prev_yaw;
} _objt;

error *est_euler_gyro_init(est_euler_gyrot **pobj, const matrix *align_dcm,
                           const vector *gyro_bias) {
    _objt *_obj = malloc(sizeof(_objt));

    _obj->align_dcm = align_dcm;
    _obj->gyro_bias = gyro_bias;
    _obj->prev_roll = 0;
    _obj->prev_pitch = 0;
    _obj->prev_yaw = 0;

    *pobj = _obj;
    return NULL;
}

void est_euler_gyro_do(est_euler_gyrot *obj, const double *gyro, double dt,
                       estimator_output *eo) {
    _objt *_obj = (_objt *)obj;

    vector tuned_gyro;
    vector_diff((const vector *)gyro, _obj->gyro_bias, &tuned_gyro);

    vector aligned_gyro;
    matrix_multiply(_obj->align_dcm, &tuned_gyro, &aligned_gyro);

    _obj->prev_roll = _obj->prev_roll + aligned_gyro.x * dt;
    _obj->prev_pitch = _obj->prev_pitch + aligned_gyro.y * dt;
    _obj->prev_yaw = _obj->prev_yaw + aligned_gyro.z * dt;

    eo->roll = _obj->prev_roll;
    eo->pitch = _obj->prev_pitch;
    // eo->yaw = _obj->prev_yaw;
    // do not use yaw, such that the result can be compared with other
    // estimators
    eo->yaw = 0;
}
