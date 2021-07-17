
#include <math.h>
#include <stdlib.h>

#include "est_dcm_compl.h"

typedef struct {
    const matrix *align_dcm;
    const vector *gyro_bias;
    double alpha;
    vector prev_K;
} _objt;

error *est_dcm_compl_init(est_dcm_complt **pobj, const matrix *align_dcm,
                          const vector *gyro_bias, double alpha) {
    _objt *_obj = malloc(sizeof(_objt));

    _obj->align_dcm = align_dcm;
    _obj->gyro_bias = gyro_bias;
    _obj->alpha = alpha;

    // initial K points downwards
    _obj->prev_K.x = 0;
    _obj->prev_K.y = 0;
    _obj->prev_K.z = 1;

    *pobj = _obj;
    return NULL;
}

void est_dcm_compl_do(est_dcm_complt *obj, const double *acc,
                      const double *gyro, double dt, estimator_output *eo) {
    _objt *_obj = (_objt *)obj;

    vector aligned_acc;
    matrix_multiply(_obj->align_dcm, (const vector *)acc, &aligned_acc);

    vector tuned_gyro;
    vector_diff((const vector *)gyro, _obj->gyro_bias, &tuned_gyro);

    vector aligned_gyro;
    matrix_multiply(_obj->align_dcm, &tuned_gyro, &aligned_gyro);

    // get K from accelerometers
    vector acc_K;
    vector_copy(&aligned_acc, &acc_K);

    // get K from gyroscopes
    vector gyro_dK;
    vector gyro_K;
    vector_cross(&_obj->prev_K, &aligned_gyro, &gyro_dK);
    vector_copy(&_obj->prev_K, &gyro_K);
    gyro_K.x += dt * gyro_dK.x;
    gyro_K.y += dt * gyro_dK.y;
    gyro_K.z += dt * gyro_dK.z;

    // filter
    _obj->prev_K.x = _obj->alpha * acc_K.x + (1 - _obj->alpha) * gyro_K.x;
    _obj->prev_K.y = _obj->alpha * acc_K.y + (1 - _obj->alpha) * gyro_K.y;
    _obj->prev_K.z = _obj->alpha * acc_K.z + (1 - _obj->alpha) * gyro_K.z;

    // normalize
    vector_normalize(&_obj->prev_K);

    eo->roll = atan2(_obj->prev_K.y, _obj->prev_K.z);
    eo->pitch = -asin(_obj->prev_K.x);
    eo->yaw = 0;
}
