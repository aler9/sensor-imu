
#pragma once

#define EST_EULER_ACC_DEFAULT_ALPHA 0.4f

typedef void est_euler_acct;

error *est_euler_acc_init(est_euler_acct **pobj, const matrix *align_dcm,
    double alpha);
void est_euler_acc_do(est_euler_acct *obj, const double *acc,
    estimator_output *eo);
