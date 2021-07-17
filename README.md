
# sensor-imu

[![Lint](https://github.com/aler9/sensor-imu/workflows/lint/badge.svg)](https://github.com/aler9/sensor-imu/actions?query=workflow:lint)

C library to interact with various IMUs (MPU6000, MPU6050, MPU6500, ICM20600, ICM20601, ICM2062). It was written for the **[raspberry-imu-viewer](https://github.com/aler9/raspberry-imu-viewer)** project, an orientation viewer in 3D for the Raspberry Pi, where you can also find a working example on how to use this library.

Features:
* works with Raspberry Pi / Linux and probably with almost every single-board computer equipped with I2C
* IMU model and address are autodetected during initialization
* sampling rate up to 1khz (when I2C speed is 400khz)
* orientation estimation algorithms are available in folder `/orientation`

## Installation

1. On the Raspberry Pi, enable I2C in fast-speed mode: edit `/boot/config.txt` and add:

   ```
   dtparam=i2c_arm=on
   dtparam=i2c_arm_baudrate=400000
   ```

   then edit `/etc/modules` and add

   ```
   i2c-dev
   ```

   then reboot the system.


2. Install the dependencies:

   ```
   sudo apt install -y git make gcc libc6-dev libi2c-dev
   ```

3. Clone this repository:

   ```
   git clone https://github.com/aler9/sensor-imu
   ```

4. Create a sample source file named `main.c`:

   ```c
   #include <stdio.h>
   #include <stdlib.h>
   #include <unistd.h>
   #include <fcntl.h>

   #include "sensor-imu/imu.h"
   #include "sensor-imu/imu_auto.h"

   int main() {
       int i2c_fd = open("/dev/i2c-1", O_RDWR);
       if(i2c_fd < 0) {
           printf("ERR: unable to open device /dev/i2c-1\n");
           return -1;
       }

       imu_autot *imu;
       error *err = imu_auto_init(&imu, i2c_fd);
       if(err != NULL) {
           printf("ERR: %s\n", err);
           return -1;
       }

       imu_output io;
       err = imu_auto_read(imu, &io);
       if(err != NULL) {
           printf("ERR: %s\n", err);
           return -1;
       }

       printf("gyro x,y,z: %f, %f, %f\n", io.gyro.x, io.gyro.y, io.gyro.z);
       printf("acc x,y,z: %f, %f, %f\n", io.acc.x, io.acc.y, io.acc.z);
       return 0;
   }
   ```

5. Build:

   ```
   gcc -o main sensor-imu/*.c main.c
   ```

6. Launch:

   ```
   ./main
   ```
