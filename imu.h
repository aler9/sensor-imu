
#pragma once

typedef const char error;

typedef enum {
    IMU_ACC_RANGE_2G,
    IMU_ACC_RANGE_4G,
    IMU_ACC_RANGE_8G,
    IMU_ACC_RANGE_16G,
} imu_acc_range;

typedef enum {
    IMU_GYRO_RANGE_250DPS,
    IMU_GYRO_RANGE_500DPS,
    IMU_GYRO_RANGE_1000DPS,
    IMU_GYRO_RANGE_2000DPS,
} imu_gyro_range;

typedef struct {
    union {
        struct {
            double x;
            double y;
            double z;
        } acc;
        double acc_array[3];
    };
    union {
        struct {
            double x;
            double y;
            double z;
        } gyro;
        double gyro_array[3];
    };
} imu_output;

typedef void imut;

error *imu_init(imut **pobj, int i2c_fd, imu_acc_range acc_range,
                imu_gyro_range gyro_range);
void imu_destroy(imut *obj);
error *imu_read(imut *obj, imu_output *out);
