
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "imu_invensense.h"

#include "imu.h"

typedef error *(*sensor_read_func)(void *, imu_output *);

typedef struct {
    int fd;
    void *sensor;
    sensor_read_func sensor_read;
} _objt;

static error *find_and_init_sensor(_objt *_obj, imu_acc_range acc_range,
                                   imu_gyro_range gyro_range) {
    for (uint8_t address = 0x68; address <= 0x69; address++) {
        int res = ioctl(_obj->fd, I2C_SLAVE, address);
        if (res != 0) {
            continue;
        }

        uint8_t cmd = 0x75;
        res = write(_obj->fd, &cmd, 1);
        if (res != 1) {
            continue;
        }

        uint8_t who_am_i;
        res = read(_obj->fd, &who_am_i, 1);
        if (res != 1) {
            continue;
        }

        switch (who_am_i) {
        case 0x68:
            printf("found MPU6000/MPU6050 with address 0x%.2x\n", address);
            break;
        case 0x70:
            printf("found MPU6500 with address 0x%.2x\n", address);
            break;
        case 0x11:
            printf("found ICM20600 with address 0x%.2x\n", address);
            break;
        case 0xAC:
            printf("found ICM20601 with address 0x%.2x\n", address);
            break;
        case 0x12:
            printf("found ICM20602 with address 0x%.2x\n", address);
            break;
        default:
            continue;
        }

        error *err = imu_invensense_init(&_obj->sensor, _obj->fd, address,
                                         acc_range, gyro_range);
        if (err != NULL) {
            return err;
        }

        _obj->sensor_read = imu_invensense_read;
        return NULL;
    }

    return "no IMU sensor found";
}

error *imu_init(imut **pobj, const char *path, imu_acc_range acc_range,
                imu_gyro_range gyro_range) {
    _objt *_obj = malloc(sizeof(_objt));

    _obj->fd = open(path, O_RDWR);
    if (_obj->fd < 0) {
        return "unable to open the device";
    }

    error *err = find_and_init_sensor(_obj, acc_range, gyro_range);
    if (err != NULL) {
        close(_obj->fd);
        free(_obj);
        return err;
    }

    *pobj = _obj;
    return NULL;
}

void imu_destroy(imut *obj) {
    _objt *_obj = (_objt *)obj;
    close(_obj->fd);
    free(obj);
}

error *imu_read(imut *obj, imu_output *out) {
    _objt *_obj = (_objt *)obj;
    return _obj->sensor_read(_obj->sensor, out);
}
