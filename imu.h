
#pragma once

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
