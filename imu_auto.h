
#pragma once

typedef void imu_autot;

error* imu_auto_init(imu_autot** pobj, int i2c_fd);
void imu_auto_destroy(imu_autot* obj);
error* imu_auto_read(imu_autot* obj, imu_output* out);
