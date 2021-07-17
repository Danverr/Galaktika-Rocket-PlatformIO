#pragma once
#include <sensors/Altimeter_Bmp280.h>
#include <sensors/Imu_Mpu6050.h>
#include <sensors/Gps_TinyGps.h>

// Перечень датчиков
extern Altimeter* ALTIMETER;
extern Imu* IMU;
extern Gps* GPS;