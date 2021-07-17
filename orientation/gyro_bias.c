
#include <string.h>

#include "../imu.h"
#include "vector.h"
#include "../imu_auto.h"
#include "gyro_bias.h"

#define SAMPLE_COUNT 500

error *gyro_bias_init(vector *gyro_bias, imu_autot *imu) {
    memset(gyro_bias, 0, sizeof(vector));
    for(int i = 0; i < SAMPLE_COUNT; i++) {
        imu_output io;
        error *err = imu_auto_read(imu, &io);
        if(err != NULL) {
            return err;
        }

        gyro_bias->x += io.gyro.x;
        gyro_bias->y += io.gyro.y;
        gyro_bias->z += io.gyro.z;
    }

    gyro_bias->x /= SAMPLE_COUNT;
    gyro_bias->y /= SAMPLE_COUNT;
    gyro_bias->z /= SAMPLE_COUNT;

    return NULL;
}
