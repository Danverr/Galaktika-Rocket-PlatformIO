#pragma once
#include "sensors/interfaces/Sensor.h"



class Gps : public Sensor {

public:

    double coords[3]; // Массив координат по XYZ
    double speed;     // Скорость в м/c
    double altitude;  // Высота в метрах

    // Обновляет данные и сохраняет их внутри объекта
    void updateData();

    // Для вывода информации о датчике
    String getSensorType() { return "GPS"; }; // Тип

};