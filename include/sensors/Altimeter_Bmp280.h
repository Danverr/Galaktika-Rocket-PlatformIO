#pragma once
#include <sensors/interfaces/Altimeter.h>
#include <Adafruit_BMP280.h>



class Altimeter_Bmp280 : public Altimeter {

public:

    // Проводит инициализацию датчика
    bool begin();

    // Обновляет данные и сохраняет их внутри объекта
    void updateData();

    // Для вывода информации о датчике
    String getSensorName() { return "BMP280"; }; // Название

private:

    Adafruit_BMP280 bmp;

    double zeroHeight = 0; // Нулевой уровень в метрах

};