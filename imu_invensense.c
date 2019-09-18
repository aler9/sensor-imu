
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#include "error.h"
#include "imu.h"
#include "imu_invensense.h"

#define POWERMAN1 0x6B
#define POWERMAN1_DISABLE_TEMP (1 << 3)
#define ACC_CONF 0x1C
#define ACC_X 0x3B
#define GYRO_CONF 0x1B
#define GYRO_X 0x43

typedef struct {
    int i2c_fd;
    uint8_t address;
    double gyro_ssf;
    double acc_ssf;
} _objt;

error* imu_invensense_init(imu_invensense** pobj, int i2c_fd, uint8_t address,
    invensense_acc_range acc_range, invensense_gyro_range gyro_range) {
    _objt* _obj = malloc(sizeof(_objt));

    _obj->i2c_fd = i2c_fd;
    _obj->address = address;

    uint8_t acc_conf = 0;

    switch(acc_range) {
    case INVENSENSE_ACC_2G:
        acc_conf = 0;
        _obj->acc_ssf = 16384.0f;
        break;

    case INVENSENSE_ACC_4G:
        acc_conf = (1 << 3);
        _obj->acc_ssf = 8192.0f;
        break;

    case INVENSENSE_ACC_8G:
        acc_conf = (2 << 3);
        _obj->acc_ssf = 4096.0f;
        break;

    case INVENSENSE_ACC_16G:
        acc_conf = (3 << 3);
        _obj->acc_ssf = 2048.0f;
        break;
    }

    uint8_t gyro_conf = 0;

    switch(gyro_range) {
    case INVENSENSE_GYRO_250:
        gyro_conf = 0;
        _obj->gyro_ssf = (131.0f * 180.0f / M_PI);
        break;

    case INVENSENSE_GYRO_500:
        gyro_conf = (1 << 3);
        _obj->gyro_ssf = (65.5f * 180.0f / M_PI);
        break;

    case INVENSENSE_GYRO_1000:
        gyro_conf = (2 << 3);
        _obj->gyro_ssf = (32.8f * 180.0f / M_PI);
        break;

    case INVENSENSE_GYRO_2000:
        gyro_conf = (3 << 3);
        _obj->gyro_ssf = (16.4f * 180.0f / M_PI);
        break;
    }

    int res = ioctl(_obj->i2c_fd, I2C_SLAVE, _obj->address);
    if(res != 0) {
        free(_obj);
        return "ioctl() failed";
    }

    uint8_t cmd[2];

    cmd[0] = POWERMAN1;
    cmd[1] = POWERMAN1_DISABLE_TEMP;
    res = write(_obj->i2c_fd, cmd, 2);
    if(res != 2) {
        free(_obj);
        return "write() failed";
    }

    cmd[0] = ACC_CONF;
    cmd[1] = acc_conf;
    res = write(_obj->i2c_fd, cmd, 2);
    if(res != 2) {
        free(_obj);
        return "write() failed";
    }

    cmd[0] = GYRO_CONF;
    cmd[1] = gyro_conf;
    res = write(_obj->i2c_fd, cmd, 2);
    if(res != 2) {
        free(_obj);
        return "write() failed";
    }

    *pobj = _obj;
    return NULL;
}

static inline int16_t make_int16(uint8_t* data) {
    return data[0] << 8 | data[1];
}

error* imu_invensense_read(void* obj, imu_output* out) {
    _objt* _obj = (_objt*)obj;

    int res = ioctl(_obj->i2c_fd, I2C_SLAVE, _obj->address);
    if(res != 0) {
        return "ioctl() failed";
    }

    uint8_t out_raw[6];

    uint8_t cmd = ACC_X;
    res = write(_obj->i2c_fd, &cmd, 1);
    if(res != 1) {
        return "write() failed";
    }

    res = read(_obj->i2c_fd, out_raw, 6);
    if(res != 6) {
        return "read() failed";
    }

    out->acc.x = - (double)make_int16(&out_raw[0]) / _obj->acc_ssf;
    out->acc.y = (double)make_int16(&out_raw[2]) / _obj->acc_ssf;
    out->acc.z = (double)make_int16(&out_raw[4]) / _obj->acc_ssf;

    cmd = GYRO_X;
    res = write(_obj->i2c_fd, &cmd, 1);
    if(res != 1) {
        return "write() failed";
    }

    res = read(_obj->i2c_fd, out_raw, 6);
    if(res != 6) {
        return "read() failed";
    }

    out->gyro.x = (double)make_int16(&out_raw[0]) / _obj->gyro_ssf;
    out->gyro.y = - (double)make_int16(&out_raw[2]) / _obj->gyro_ssf;
    out->gyro.z = (double)make_int16(&out_raw[4]) / _obj->gyro_ssf;

    return NULL;
}
