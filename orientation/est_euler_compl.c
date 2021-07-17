
#include <stdlib.h>
#include <math.h>

#include "../imu.h"
#include "vector.h"
#include "est.h"
#include "est_euler_compl.h"

typedef struct {
    const matrix *align_dcm;
    const vector *gyro_bias;
    double alpha;
    double prev_roll;
    double prev_pitch;
} _objt;

error *est_euler_compl_init(est_euler_complt **pobj, const matrix *align_dcm,
    const vector *gyro_bias, double alpha) {
    _objt *_obj = malloc(sizeof(_objt));

    _obj->align_dcm = align_dcm;
    _obj->gyro_bias = gyro_bias;
    _obj->alpha = alpha;
    _obj->prev_roll = 0;
    _obj->prev_pitch = 0;

    *pobj = _obj;
    return NULL;
}

void est_euler_compl_do(est_euler_complt *obj, const double *acc, const double *gyro, double dt,
    estimator_output *eo) {
    _objt *_obj = (_objt*)obj;

    vector aligned_acc;
    matrix_multiply(_obj->align_dcm, (const vector*)acc, &aligned_acc);

    vector tuned_gyro;
    vector_diff((const vector*)gyro, _obj->gyro_bias, &tuned_gyro);

    vector aligned_gyro;
    matrix_multiply(_obj->align_dcm, &tuned_gyro, &aligned_gyro);

    double acc_roll = atan2(aligned_acc.y, aligned_acc.z);
    double acc_pitch = -atan2(aligned_acc.x, sqrt(aligned_acc.y*aligned_acc.y + aligned_acc.z*aligned_acc.z));

    _obj->prev_roll = acc_roll*_obj->alpha + (1 - _obj->alpha)*(_obj->prev_roll + aligned_gyro.x * dt);
    _obj->prev_pitch = acc_pitch*_obj->alpha + (1 - _obj->alpha)*(_obj->prev_pitch + aligned_gyro.y * dt);

    eo->roll = _obj->prev_roll;
    eo->pitch = _obj->prev_pitch;
    eo->yaw = 0;
}
