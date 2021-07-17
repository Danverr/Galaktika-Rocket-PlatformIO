#pragma once
#include "Sensor.h"
#include <Kalman.h>



class Imu : public Sensor {

public:

    double acceleration[3]; // Ускорение в G по XYZ
    double angularSpeed[3]; // Угловая скорсть в град/c по XYZ
    double eulerAngles[3];  // Углы Эйлера по XYZ в градусах

    // Обновляет данные и сохраняет их внутри объекта
    void updateData();

    // Для вывода информации о датчике
    String getSensorType() { return "IMU"; }; // Тип

};