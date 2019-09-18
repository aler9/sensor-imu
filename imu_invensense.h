
#pragma once

typedef enum {
    INVENSENSE_ACC_2G,
    INVENSENSE_ACC_4G,
    INVENSENSE_ACC_8G,
    INVENSENSE_ACC_16G,
} invensense_acc_range;

typedef enum {
    INVENSENSE_GYRO_250,
    INVENSENSE_GYRO_500,
    INVENSENSE_GYRO_1000,
    INVENSENSE_GYRO_2000,
} invensense_gyro_range;

typedef void imu_invensense;

error* imu_invensense_init(imu_invensense** pobj, int i2c_fd, uint8_t address,
    invensense_acc_range acc_range, invensense_gyro_range gyro_range);
error* imu_invensense_read(void* obj, imu_output* r);
