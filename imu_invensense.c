
#include <linux/i2c-dev.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "imu.h"

#include "imu_invensense.h"

#define POWERMAN1 0x6B
#define POWERMAN1_DISABLE_TEMP (1 << 3)
#define ACC_CONF 0x1C
#define ACC_X 0x3B
#define GYRO_CONF 0x1B
#define GYRO_X 0x43

typedef struct {
    int fd;
    uint8_t address;
    double gyro_ssf;
    double acc_ssf;
} _objt;

error *imu_invensense_init(imu_invensense **pobj, int fd, uint8_t address,
                           imu_acc_range acc_range, imu_gyro_range gyro_range) {
    _objt *_obj = malloc(sizeof(_objt));

    _obj->fd = fd;
    _obj->address = address;

    uint8_t acc_conf = 0;

    switch (acc_range) {
    case IMU_ACC_RANGE_2G:
        acc_conf = 0;
        _obj->acc_ssf = 32768.0f / 2.0f;
        break;

    case IMU_ACC_RANGE_4G:
        acc_conf = (1 << 3);
        _obj->acc_ssf = 32768.0f / 4.0f;
        break;

    case IMU_ACC_RANGE_8G:
        acc_conf = (2 << 3);
        _obj->acc_ssf = 32768.0f / 8.0f;
        break;

    case IMU_ACC_RANGE_16G:
        acc_conf = (3 << 3);
        _obj->acc_ssf = 32768.0f / 16.0f;
        break;
    }

    uint8_t gyro_conf = 0;

    switch (gyro_range) {
    case IMU_GYRO_RANGE_250DPS:
        gyro_conf = 0;
        _obj->gyro_ssf = 32768.0f / 250.0f;
        break;

    case IMU_GYRO_RANGE_500DPS:
        gyro_conf = (1 << 3);
        _obj->gyro_ssf = 32768.0f / 500.0f;
        break;

    case IMU_GYRO_RANGE_1000DPS:
        gyro_conf = (2 << 3);
        _obj->gyro_ssf = 32768.0f / 1000.0f;
        break;

    case IMU_GYRO_RANGE_2000DPS:
        gyro_conf = (3 << 3);
        _obj->gyro_ssf = 32768.0f / 2000.0f;
        break;
    }

    int res = ioctl(_obj->fd, I2C_SLAVE, _obj->address);
    if (res != 0) {
        free(_obj);
        return "ioctl() failed";
    }

    uint8_t cmd[2];

    cmd[0] = POWERMAN1;
    cmd[1] = POWERMAN1_DISABLE_TEMP;
    res = write(_obj->fd, cmd, 2);
    if (res != 2) {
        free(_obj);
        return "write() failed";
    }

    cmd[0] = ACC_CONF;
    cmd[1] = acc_conf;
    res = write(_obj->fd, cmd, 2);
    if (res != 2) {
        free(_obj);
        return "write() failed";
    }

    cmd[0] = GYRO_CONF;
    cmd[1] = gyro_conf;
    res = write(_obj->fd, cmd, 2);
    if (res != 2) {
        free(_obj);
        return "write() failed";
    }

    *pobj = _obj;
    return NULL;
}

void imu_invensense_destroy(imu_invensense *obj) { free(obj); }

static inline int16_t make_int16(uint8_t *data) {
    return data[0] << 8 | data[1];
}

error *imu_invensense_read(void *obj, imu_output *out) {
    _objt *_obj = (_objt *)obj;

    uint8_t out_raw[6];

    uint8_t cmd = ACC_X;
    int res = write(_obj->fd, &cmd, 1);
    if (res != 1) {
        return "write() failed";
    }

    res = read(_obj->fd, out_raw, 6);
    if (res != 6) {
        return "read() failed";
    }

    out->acc.x = -(double)make_int16(&out_raw[0]) / _obj->acc_ssf;
    out->acc.y = (double)make_int16(&out_raw[2]) / _obj->acc_ssf;
    out->acc.z = (double)make_int16(&out_raw[4]) / _obj->acc_ssf;

    cmd = GYRO_X;
    res = write(_obj->fd, &cmd, 1);
    if (res != 1) {
        return "write() failed";
    }

    res = read(_obj->fd, out_raw, 6);
    if (res != 6) {
        return "read() failed";
    }

    out->gyro.x = (double)make_int16(&out_raw[0]) / _obj->gyro_ssf;
    out->gyro.y = -(double)make_int16(&out_raw[2]) / _obj->gyro_ssf;
    out->gyro.z = (double)make_int16(&out_raw[4]) / _obj->gyro_ssf;

    return NULL;
}
