
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "../imu.h"
#include "../imu_auto.h"

int main() {
    // disable stdout buffering
    setbuf(stdout, NULL);

    int i2c_fd = open("/dev/i2c-1", O_RDWR);
    if(i2c_fd < 0) {
        return -1;
    }

    imu_autot* imu;
    error* err = imu_auto_init(&imu, i2c_fd);
    if(err != NULL) {
        return -1;
    }

    while(1) {
        imu_output io;
        err = imu_auto_read(imu, &io);
        if(err != NULL) {
            return -1;
        }

        printf("gyro x,y,z: %f, %f, %f\n", io.gyro.x, io.gyro.y, io.gyro.z);
        printf("acc x,y,z: %f, %f, %f\n", io.acc.x, io.acc.y, io.acc.z);
    }

    return 0;
}
