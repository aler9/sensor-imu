
#include <stdio.h>
#include <stdlib.h>

#include "../imu.h"

int main() {
    // disable stdout buffering
    setbuf(stdout, NULL);

    imut *imu;
    error *err =
        imu_init(&imu, "/dev/i2c-1", IMU_ACC_RANGE_2G, IMU_GYRO_RANGE_250DPS);
    if (err != NULL) {
        return -1;
    }

    while (1) {
        imu_output io;
        err = imu_read(imu, &io);
        if (err != NULL) {
            return -1;
        }

        printf("gyro x,y,z: %f, %f, %f\n", io.gyro.x, io.gyro.y, io.gyro.z);
        printf("acc x,y,z: %f, %f, %f\n", io.acc.x, io.acc.y, io.acc.z);
    }

    return 0;
}
