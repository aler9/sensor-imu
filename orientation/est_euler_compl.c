
#include <stdlib.h>
#include <math.h>

#include "../error.h"
#include "vector.h"
#include "../imu.h"
#include "../imu_auto.h"
#include "align_dcm.h"
#include "gyro_bias.h"
#include "est.h"
#include "est_euler_compl.h"

#define ALPHA 0.1f

typedef struct {
    vector gyro_bias;
    matrix align;
    double prev_roll;
    double prev_pitch;
} _objt;

error* est_euler_compl_init(est_euler_complt** pobj, imu_autot* imu) {
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

    _obj->prev_roll = 0;
    _obj->prev_pitch = 0;

    *pobj = _obj;
    return NULL;
}

void est_euler_compl_do(est_euler_complt* obj, const double* acc, const double* gyro, double dt,
    estimator_output* eo) {
    _objt* _obj = (_objt*)obj;

    vector aligned_acc;
    matrix_multiply(&_obj->align, (const vector*)acc, &aligned_acc);

    vector tuned_gyro;
    vector_diff((const vector*)gyro, &_obj->gyro_bias, &tuned_gyro);

    vector aligned_gyro;
    matrix_multiply(&_obj->align, &tuned_gyro, &aligned_gyro);

    double acc_roll = atan2(aligned_acc.y, aligned_acc.z);
    double acc_pitch = -atan2(aligned_acc.x, sqrt(aligned_acc.y*aligned_acc.y + aligned_acc.z*aligned_acc.z));

    _obj->prev_roll = acc_roll*ALPHA + (1 - ALPHA)*(_obj->prev_roll + aligned_gyro.x * dt);
    _obj->prev_pitch = acc_pitch*ALPHA + (1 - ALPHA)*(_obj->prev_pitch + aligned_gyro.y * dt);

    eo->roll = _obj->prev_roll;
    eo->pitch = _obj->prev_pitch;
    eo->yaw = 0;
}
