
#include <stdlib.h>
#include <math.h>

#include "../error.h"
#include "vector.h"
#include "../imu.h"
#include "../imu_auto.h"
#include "align_dcm.h"
#include "gyro_bias.h"
#include "est.h"
#include "est_dcm_compl.h"

#define ALPHA 0.1f

typedef struct {
    vector gyro_bias;
    matrix align;
    vector prev_K;
} _objt;

error* est_dcm_compl_init(est_dcm_complt** pobj, imu_autot* imu) {
    _objt* _obj = malloc(sizeof(_objt));

    error* err = gyro_bias_init(&_obj->gyro_bias, imu);
    if(err != NULL) {
        free(_obj);
        return err;
    }

    err = align_dcm_init(&_obj->align, imu);
    if(err != NULL) {
        free(_obj);
        return err;
    }

    // initial K points downwards
    _obj->prev_K.x = 0;
    _obj->prev_K.y = 0;
    _obj->prev_K.z = 1;

    *pobj = _obj;
    return NULL;
}

void est_dcm_compl_do(est_dcm_complt* obj, const double* acc, const double* gyro, double dt,
    estimator_output* eo) {
    _objt* _obj = (_objt*)obj;

    vector aligned_acc;
    matrix_multiply(&_obj->align, (const vector*)acc, &aligned_acc);

    vector tuned_gyro;
    vector_diff((const vector*)gyro, &_obj->gyro_bias, &tuned_gyro);

    vector aligned_gyro;
    matrix_multiply(&_obj->align, &tuned_gyro, &aligned_gyro);

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
    _obj->prev_K.x = ALPHA*acc_K.x + (1 - ALPHA)*gyro_K.x;
    _obj->prev_K.y = ALPHA*acc_K.y + (1 - ALPHA)*gyro_K.y;
    _obj->prev_K.z = ALPHA*acc_K.z + (1 - ALPHA)*gyro_K.z;

    // normalize
    vector_normalize(&_obj->prev_K);

    eo->roll = atan2(_obj->prev_K.y, _obj->prev_K.z);
    eo->pitch = -asin(_obj->prev_K.x);
    eo->yaw = 0;
}
