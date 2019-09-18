
#include <stdlib.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <stdio.h>
#include <unistd.h>

#include "error.h"
#include "imu.h"
#include "imu_invensense.h"
#include "imu_auto.h"

typedef error* (*sensor_read_func)(void*, imu_output*);

typedef struct {
    void* sensor;
    sensor_read_func sensor_read;
} _objt;

static error* find_sensor(_objt* _obj, int i2c_fd) {
    for(uint8_t address = 0x68; address <= 0x69; address++) {
        int res = ioctl(i2c_fd, I2C_SLAVE, address);
        if(res != 0) {
            continue;
        }

        uint8_t cmd = 0x75;
        res = write(i2c_fd, &cmd, 1);
        if(res != 1) {
            continue;
        }

        uint8_t who_am_i;
        res = read(i2c_fd, &who_am_i, 1);
        if(res != 1) {
            continue;
        }

        switch(who_am_i) {
        case 0x68: printf("found MPU6000/MPU6050 with address 0x%.2x\n", address); break;
        case 0x70: printf("found MPU6500 with address 0x%.2x\n", address); break;
        case 0x11: printf("found ICM20600 with address 0x%.2x\n", address); break;
        case 0xAC: printf("found ICM20601 with address 0x%.2x\n", address); break;
        case 0x12: printf("found ICM20602 with address 0x%.2x\n", address); break;
        default: continue;
        }

        imu_invensense_init(&_obj->sensor, i2c_fd, address, INVENSENSE_ACC_2G, INVENSENSE_GYRO_250);
        _obj->sensor_read = imu_invensense_read;
        return NULL;
    }

    return "no IMU sensor found";
}

error* imu_auto_init(imu_autot** pobj, int i2c_fd) {
    _objt* _obj = malloc(sizeof(_objt));

    error* err = find_sensor(_obj, i2c_fd);
    if(err != NULL) {
        free(_obj);
        return err;
    }

    *pobj = _obj;
    return NULL;
}

error* imu_auto_read(imu_autot* obj, imu_output* out) {
    _objt* _obj = (_objt*)obj;
    return _obj->sensor_read(_obj->sensor, out);
}
