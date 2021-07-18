
#pragma once

#include "imu.h"

typedef void imu_invensense;

error *imu_invensense_init(imu_invensense **pobj, int fd, uint8_t address,
                           imu_acc_range acc_range, imu_gyro_range gyro_range);
void imu_invensense_destroy(imu_invensense *obj);
error *imu_invensense_read(void *obj, imu_output *r);
