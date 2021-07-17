#pragma once
#include <Wire.h>



class Sensor {

public:

    // Проводит инициализацию датчика
    virtual bool begin() = 0;

    // Обновляет данные и сохраняет их внутри объекта
    virtual void updateData() = 0;

    // Для вывода информации о датчике
    String getSensorName() { return "Unknown"; }; // Название
    String getSensorType() { return "Unknown"; }; // Тип

    // Калибровка датчика
    virtual void calibrate() {};

    // Принимает и обрабатывает байт из Serial.read()
    virtual void encodeSerial(int b) {};

};