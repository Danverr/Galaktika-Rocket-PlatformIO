#include <Arduino.h>
#include <TeensyThreads.h>
#include "config.h"
#include "ImportSensors.h"
#include "states/InitState.h"

void setup() {
    Serial.begin(9600);     // Общение с платой по проводу
    rfSerial.begin(115200); // Радиоканал
    gpsSerial.begin(9600);  // Шина GPS

    ALTIMETER = new Altimeter_Bmp280();
    IMU = new Imu_Mpu6050();
    GPS = new Gps_TinyGps();
    ROCKET_STATE = new InitState();
}

// Прием данных по шине GPS
void serialEvent6() {
    GPS->encodeSerial(Serial6.read());
}

// Прием данных по радиосвязи
void serialEvent3() {

}

void loop() {
    ROCKET_STATE->execute();
}