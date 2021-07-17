#include "sensors/Imu_Mpu6050.h"



bool Imu_Mpu6050::begin() {
    if(mpu6050.testConnection() == false){
        return false;
    }

    Wire.begin();

    for (int i = 0; i < 3; i++)
        kalmanFilter[i].setAngle(180);

    timer = micros();
    mpu6050.initialize();

    mpu6050.setXGyroOffset(50);
    mpu6050.setZGyroOffset(50);
    mpu6050.setFullScaleAccelRange(MPU6050_ACCEL_FS);

    return true;
}



void Imu_Mpu6050::updateData() {
    int16_t tempAcc[3];
    int16_t tempGyro[3];
    double accAngle[3];
    double gyroRate[3];
    double kalmanAngle[3];

    // Получаем данные с датчика
    mpu6050.getMotion6(&tempAcc[0], &tempAcc[1], &tempAcc[3], &tempGyro[0], &tempGyro[1], &tempGyro[2]);

    // Отнимаем ошибку, сохраняем ускорение и угловую скорость
    for (int i = 0; i < 3; i++) {
        tempGyro[i] -= gyroError[i];
        angularSpeed[i] = (double)tempGyro[i] / 32768.0 * getGyroMaxValue();
        acceleration[i] = (double)tempAcc[i]  / 32768.0 * getAccelMaxValue();
    }

    // Рассчет углов с помощью акселерометра
    accAngle[0] = (atan2(tempAcc[1], tempAcc[2]) + PI) * RAD_TO_DEG;
    accAngle[1] = (atan2(tempAcc[0], tempAcc[2]) + PI) * RAD_TO_DEG;
    accAngle[2] = (atan2(tempAcc[0], tempAcc[1]) + PI) * RAD_TO_DEG;

    gyroRate[0] = (double)tempGyro[0] / 131.0;
    gyroRate[1] = -(double)tempGyro[1] / 131.0;
    gyroRate[2] = (double)tempGyro[2] / 131.0;

    // Calculate gyro angle using the unbiased rate
    for (int i = 0; i < 3; i++) {
        gyroRate[i] += kalmanFilter[i].getRate() * ((double)(micros() - timer) / 1000000);

        if(USE_ACCEL_CORRECTION){
            kalmanAngle[i] = kalmanFilter[i].getAngle(accAngle[i], gyroRate[i], (double)(micros() - timer) / 1000000);
        } else {
            kalmanAngle[i] += gyroRate[i] * (double)(micros() - timer) / 1000000;
        }
    }

    timer = micros();

    eulerAngles[0] = (kalmanAngle[0]) * cos(AXIS_ROTATION * DEG_TO_RAD) - (kalmanAngle[2]) * sin(AXIS_ROTATION * DEG_TO_RAD);
    eulerAngles[1] = kalmanAngle[1] - offsetAngle[1];
    eulerAngles[2] = (kalmanAngle[0]) * sin(AXIS_ROTATION * DEG_TO_RAD) + (kalmanAngle[2]) * cos(AXIS_ROTATION * DEG_TO_RAD);
    eulerAngles[0] = fmod(kalmanAngle[0] + 540 - offsetAngle[0], 360) - 180;
    eulerAngles[2] = fmod(kalmanAngle[2] + 540 - offsetAngle[2], 360) - 180;
}



double Imu_Mpu6050::getAccelMaxValue() {
    switch (MPU6050_ACCEL_FS) {
        case MPU6050_ACCEL_FS_2: return 2.0;
        case MPU6050_ACCEL_FS_4: return 4.0;
        case MPU6050_ACCEL_FS_8: return 8.0;
        case MPU6050_ACCEL_FS_16: return 16.0;    
        default: return 2.0;
    }
}



double Imu_Mpu6050::getGyroMaxValue() {
    switch (MPU6050_GYRO_FS) {
        case MPU6050_GYRO_FS_250: return 250.0;
        case MPU6050_GYRO_FS_500: return 500.0;
        case MPU6050_GYRO_FS_1000: return 1000.0;
        case MPU6050_GYRO_FS_2000: return 2000.0;    
        default: return 250.0;
    }
}