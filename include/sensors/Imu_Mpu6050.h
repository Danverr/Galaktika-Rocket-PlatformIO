#pragma once
#include <sensors/interfaces/Imu.h>
#include <MPU6050.h>



class Imu_Mpu6050 : public Imu {

public:

    // Проводит инициализацию датчика
    bool begin();

    // Обновляет данные и сохраняет их внутри объекта
    void updateData();

    // Для вывода информации о датчике
    String getSensorName() { return "MPU6050"; }; // Название

private:

    // Константы и настройки
    const PROGMEM bool USE_ACCEL_CORRECTION = true;          // Использовать ли акселерометр при вычислении углов
    const PROGMEM double AXIS_ROTATION = 0.0;                // ???
    const PROGMEM int MPU6050_ACCEL_FS = MPU6050_ACCEL_FS_8; // Предел измерений акселерометра
    const PROGMEM int MPU6050_GYRO_FS = MPU6050_GYRO_FS_500; // Предел измерений гироскопа

    unsigned long timer = 0;             // Таймер для дифференцирования в микросекундах
    MPU6050 mpu6050;                     // Объект датчика MPU6050

    int16_t gyroError[3] = { 0, 0, 0 };  // Ошибка гироскопа для угловой скорости

    Kalman kalmanFilter[3];              // Фильтр калмана по XYZ для углов Эйлера
    double offsetAngle[3] = { 0, 0, 0 }; // Сдвиг для коррекции углов Эйлера

    double getAccelMaxValue();           // Максимальное значение акселерометра в G
    double getGyroMaxValue();            // Максимальное значение гироскопа в град/с

};