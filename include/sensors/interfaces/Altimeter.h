#pragma once
#include "Sensor.h"



class Altimeter : public Sensor {

public:

    double altitude; // Высота в метрах

    // Обновляет данные и сохраняет их внутри объекта
    void updateData();

    // Для вывода информации о датчике
    String getSensorType() { return "Altimeter"; }; // Тип

};