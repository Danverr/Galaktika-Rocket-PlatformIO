#include "BlipSystem.h"
#include "SystemLogger.h"
#include "Arduino.h"

void SystemLogger::begin(int time) {
    Serial3.write(RF_START_MARKER);    
    
    String str = "\n" + (String)time;
    if (file) file.print(str);
    Serial.print(str);
}

void SystemLogger::log(byte bytes[], int n) {
    for (int i = 0; i < n; i++) {
        if (bytes[i] == RF_START_MARKER || bytes[i] == RF_ESCAPE_MARKER) {
            Serial3.write(RF_ESCAPE_MARKER);
        }
        Serial3.write(bytes[i]);
    }
}

void SystemLogger::log(long val) {
    String str = ',' + (String)val;    
    if (file) file.print(str);
    Serial.print(str);
    log((byte*)&val, RF_VAR_SIZE);
}

void SystemLogger::log(int val) {
    log(long(val));
}

void SystemLogger::log(float val) {
    String str = ',' + (String)val;    
    if (file) file.print(str);
    Serial.print(str);
    log((byte*)&val, RF_VAR_SIZE);
}

void SystemLogger::log(double val) {
    log(float(val));
}

SystemLogger::SystemLogger(BlipSystem* pBS, unsigned long _logDelta = 50, char* _fileName = "data.txt")
    : pBlipSystem(pBS), logDelta(_logDelta), fileName(fileName) {
}

bool SystemLogger::start() {   
    if (SD.begin(SD_CARD)) {
        file = SD.open("data.txt", FILE_WRITE);
        if (!file) {
            Serial.println("ERROR: SD-CARD CAN'T OPEN FILE");
        }
        startTimer = millis();
        return file;
    }

    Serial.println("ERROR: SD-CARD ISN'T OPEN");    
    return false;
}

void SystemLogger::logInfo() {
    if (millis() - logTimer >= logDelta) {
        logTimer = millis();

        int time = (millis() - startTimer);
        String state = pBlipSystem->getState()->getId();
        double height = pBlipSystem->getHeight();
        double gyroX = pBlipSystem->getGyroX(); // X = Pitch
        double gyroY = pBlipSystem->getGyroY(); // Y = Yaw
        double gyroZ = pBlipSystem->getGyroZ(); // Z = Roll
        double angularSpeedX = pBlipSystem->getAngularSpeedX();
        double angularSpeedY = pBlipSystem->getAngularSpeedY();
        double angularSpeedZ = pBlipSystem->getAngularSpeedZ();
        long accX = pBlipSystem->getAccX();
        long accY = pBlipSystem->getAccY();
        long accZ = pBlipSystem->getAccZ();
        double coordX = pBlipSystem->getGps().location.lat();
        double coordY = pBlipSystem->getGps().location.lng();
        double coordZ = pBlipSystem->getGps().altitude.meters(); 
        double speed = pBlipSystem->getGps().speed.mps();
        int servoX = pBlipSystem->getServoXPointer()->read();
        int servoY = pBlipSystem->getServoZPointer()->read();

        // Передача по радиоканалу
        begin(time);

        //  1: Идентификатор запроса                 
        //  2: Сам ответ
        for (int i = 0; i < RF_RESPONSE_SIZE; i++)
            log(requestHandler->getResponse()[i]);

        log(currentStage); // 3: Стадия полета   
        log(height); // 4: Высота                                
        log(speed); // 5: Скорость в м/с                   
        log(accY / 2048.0); // 6: Ускорение по Oy в G                  
        log(accX / 2048.0); // 7: Ускорение по Ox в G                      
        log(accZ / 2048.0); // 8: Ускорение по Oz в G                            
        log(coordX); // 9: Координата GPS по Ox                  
        log(coordY); // 10: Координата GPS по Oy                  
        log(coordZ); // 11: Координата GPS по Oz                          
        log(gyroZ); // 12: Yaw (Рыскание) в градусах                   
        log(gyroX); // 13: Pitch (Тангаж) в градусах             
        log(gyroY); // 14: Roll (Крен) в градусах                       
        log(angularSpeedZ); // 15: Скорость рыскания в градусах в секунду
        log(angularSpeedX); // 16: Скорость тангажа в градусах в секунду 
        log(angularSpeedY); // 17: Скорость крена в градусах в секунду   
        log(servoX); // 18: Координата X серво
        log(servoY); // 19: Координата Y серво        

        // Вывод в Serial порт по проводу
        /*
        Serial.println("ACCEL:");
        Serial.println(" aX : " + (String)accX);
        Serial.println(" aY : " + (String)accY);
        Serial.println(" aZ : " + (String)accZ);
        Serial.println("GYRO:");
        Serial.println(" gX : " + (String)gyroX);
        Serial.println(" gY : " + (String)gyroY);
        Serial.println(" gZ : " + (String)gyroZ);
        Serial.println("M: " + (String)height);
        Serial.println("ZH: " + (String)zeroHeight);
        Serial.println("STATE: " + (String)state);
        */
    }
}

void SystemLogger::logEvent(String eventName) {
    if (file) {
        String msg = "\n# LOG TIME: " + (String)(millis() - startTimer) +  ", EVENT: " + eventName;
        file.println(msg);
    }
}

void SystemLogger::setStage(FlightStages stage) {
    currentStage = stage;
}

void SystemLogger::finish() {
    file.close();
}
