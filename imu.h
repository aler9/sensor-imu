
#pragma once

typedef struct {
    struct {
        double x;
        double y;
        double z;
    } acc;
    struct {
        double x;
        double y;
        double z;
    } gyro;
} imu_output;
