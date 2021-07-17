
#include <stdlib.h>
#include <math.h>

#include "../imu.h"
#include "vector.h"
#include "est.h"
#include "est_euler_acc.h"

typedef struct {
    const matrix *align_dcm;
    double alpha;
    double prev_roll;
    double prev_pitch;
} _objt;

error *est_euler_acc_init(est_euler_acct **pobj, const matrix *align_dcm, double alpha) {
    _objt *_obj = malloc(sizeof(_objt));

    _obj->align_dcm = align_dcm;
    _obj->alpha = alpha; // normally is 0.4
    _obj->prev_roll = 0;
    _obj->prev_pitch = 0;

    *pobj = _obj;
    return NULL;
}

void est_euler_acc_do(est_euler_acct *obj, const double *acc, estimator_output *eo) {
    _objt *_obj = (_objt*)obj;

    vector aligned_acc;
    matrix_multiply(_obj->align_dcm, (const vector*)acc, &aligned_acc);

    double cur_roll = atan2(aligned_acc.y, aligned_acc.z);
    double cur_pitch = -atan2(aligned_acc.x, sqrt(aligned_acc.y*aligned_acc.y + aligned_acc.z*aligned_acc.z));

    _obj->prev_roll = _obj->alpha*cur_roll + _obj->prev_roll*(1 - _obj->alpha);
    _obj->prev_pitch = _obj->alpha*cur_pitch + _obj->prev_pitch*(1 - _obj->alpha);

    eo->roll = _obj->prev_roll;
    eo->pitch = _obj->prev_pitch;
    eo->yaw = 0;
}
