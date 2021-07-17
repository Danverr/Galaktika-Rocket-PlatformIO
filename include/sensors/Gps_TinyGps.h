#pragma once
#include <sensors/interfaces/Gps.h>
#include <TinyGPS++.h>



class Gps_TinyGps : public Gps {

public:

    // Проводит инициализацию датчика
    bool begin();

    // Обновляет данные и сохраняет их внутри объекта
    void updateData();

    // Для вывода информации о датчике
    String getSensorName() { return "TinyGps"; }; // Название

    // Принимает и обрабатывает байт из Serial.read()
    void encodeSerial(int b);

private:

    TinyGPSPlus tinyGps;

};