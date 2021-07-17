
#pragma once

typedef const char error;

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

error *imu_init(imut **pobj, int i2c_fd);
void imu_destroy(imut *obj);
error *imu_read(imut *obj, imu_output *out);
