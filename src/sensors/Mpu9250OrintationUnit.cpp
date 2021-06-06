#include "./sensors/Mpu9250OrintationUnit.h"

#include <Wire.h>
#include "Arduino.h"

#include "BlipSystem.h"
#include "BlipSubscriber.h"

Mpu9250OrintationUnit::Mpu9250OrintationUnit(float axesRotation = 0, bool accelCorrection = true)
    : OrintationUnit(axesRotation, accelCorrection) {
    Wire.begin();
    accelgyro.setup(0x68);
    kalmanX.setAngle(180); // Set starting angle
    kalmanY.setAngle(180);
    kalmanZ.setAngle(180);
    timer = micros();
    // accelgyro.dmpInitialize();
    // Serial.println("Testing MPU6050 connections...");
    // Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
}

void Mpu9250OrintationUnit::getMetrics6(double* pAngleX, double* pAngleY, double* pAngleZ, long* pAccX, long* pAccY, long* pAccZ) {
    accelgyro.update();
    rawAccX = accelgyro.getAccX();
    rawAccY = accelgyro.getAccY();
    rawAccZ = accelgyro.getAccZ();
    rawGyroX = accelgyro.getGyroX();
    rawGyroY = accelgyro.getGyroY();
    rawGyroZ = accelgyro.getGyroZ();
    int16_t accX = rawAccX;
    int16_t accY = rawAccY;
    int16_t accZ = rawAccZ;
    int16_t gyroX = rawGyroX - gyroErrorX;
    int16_t gyroY = rawGyroY - gyroErrorY;
    int16_t gyroZ = rawGyroZ - gyroErrorZ;

    // Calculate the angles based on the different sensors and algorithm

    // Angle calculate using the accelerometer
    double accYangle = (atan2(accX, accZ) + PI) * RAD_TO_DEG;
    double accXangle = (atan2(accY, accZ) + PI) * RAD_TO_DEG;
    double accZangle = (atan2(accX, accY) + PI) * RAD_TO_DEG;
    gyroXrate = (double)gyroX / 131.0;
    gyroYrate = -(double)gyroY / 131.0;
    gyroZrate = (double)gyroZ / 131.0;

    // Calculate gyro angle using the unbiased rate
    gyroXangle += kalmanX.getRate() * ((double)(micros() - timer) / 1000000);
    gyroYangle += kalmanY.getRate() * ((double)(micros() - timer) / 1000000);
    gyroZangle += kalmanZ.getRate() * ((double)(micros() - timer) / 1000000);

    if (accelCorrection) {
        // Calculate the angle using a Kalman filter
        kalAngleX = kalmanX.getAngle(accXangle, gyroXrate, (double)(micros() - timer) / 1000000);
        kalAngleY = kalmanY.getAngle(accYangle, gyroYrate, (double)(micros() - timer) / 1000000);
        kalAngleZ = kalmanZ.getAngle(accZangle, gyroZrate, (double)(micros() - timer) / 1000000);
    } else {
        kalAngleX += gyroXrate * (double)(micros() - timer) / 1000000;
        kalAngleY += gyroYrate * (double)(micros() - timer) / 1000000;
        kalAngleZ += gyroZrate * (double)(micros() - timer) / 1000000;
    }

    timer = micros();
    *pAngleX = (kalAngleX)*cos(axesRotation * DEG_TO_RAD) -
        (kalAngleZ)*sin(axesRotation * DEG_TO_RAD);
    *pAngleY = kalAngleY - offsetY;
    *pAngleZ = (kalAngleX)*sin(axesRotation * DEG_TO_RAD) +
        (kalAngleZ)*cos(axesRotation * DEG_TO_RAD);
    *pAngleX = fmod(*pAngleX + 540 - offsetX, 360) - 180;
    *pAngleZ = fmod(*pAngleZ + 540 - offsetZ, 360) - 180;
    /*
    *pAngleX = (kalAngleX) * cos(axesRotation * DEG_TO_RAD) -
               (kalAngleZ) * sin(axesRotation * DEG_TO_RAD) - offsetX;
    *pAngleY = kalAngleY - offsetY;
    *pAngleZ = (kalAngleX) * sin(axesRotation * DEG_TO_RAD) +
               (kalAngleZ) * cos(axesRotation * DEG_TO_RAD) - offsetZ;
    */
    *pAccX = accX;
    *pAccY = accY;
    *pAccZ = accZ;
    // The accelerometer's maximum samples rate is 1kHz
}

void Mpu9250OrintationUnit::getAngularSpeed(double& angularSpeedX, double& angularSpeedY, double& angularSpeedZ) {
    gyroXrate = angularSpeedX;
    gyroYrate = angularSpeedY;
    gyroZrate = angularSpeedZ;
}

void Mpu9250OrintationUnit::resetAngles() {
    kalAngleX = offsetX * cos(axesRotation * DEG_TO_RAD) + offsetZ * sin(axesRotation * DEG_TO_RAD);
    kalAngleY = offsetY;
    kalAngleZ = offsetZ * cos(axesRotation * DEG_TO_RAD) - offsetX * sin(axesRotation * DEG_TO_RAD);
    Serial.println("Agles were reseted");
    Serial.println(kalAngleX);
    Serial.println(kalAngleY);
    Serial.println(kalAngleZ);
    // delay(3000);
}
