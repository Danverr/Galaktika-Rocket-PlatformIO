#include <bmp3.h>
#include <bmp3_defs.h>

#ifndef _BLIP_SYSTEM_BLIP_H_
#define _BLIP_SYSTEM_BLIP_H_

#include <SPI.h>
#include <Wire.h>
#include <Servo.h>
#include <Vector.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_BMP3XX.h>
#include "TinyGPS++.h"

#include "./utils/LinkedList.h"
#include "./utils/Pair.h"

#include "config.h"
#include "BlipSubscriber.h"
#include "BlipState.h"
#include "./states/LandingState.h"
#include "./states/SetupState.h"
#include "./states/WaitingStartState.h"
#include "SystemLogger.h"
#include "./sensors/OrintationUnit.h"
#include "./sensors/MpuOrintationUnit.h"
#include "IEventManager.h"

//State codes
#define SETUP 8
#define WAITING_START 1
#define MIDDLE_AIR 2
#define LANDING 3
#define SAVING_STATE 6
#define FINAL_STATE 4
//Event codes
#define EVENT_GOAL_HAD_REACHED 22
#define EVENT_MISSION_ABORT 10

#define TO_DEG 57.2957f
#define TIME_OUT 20
#define BETA 0.22f
#define FLASH_SS 8
#define MPU 0x68
/*
  #define BMP_SCK  (13)
  #define BMP_MISO (12)
  #define BMP_MOSI (11)
  #define BMP_CS   (10)*/

#define PARACHUTE_PIN_1 22
#define PARACHUTE_PIN_2 23
#define SD_CARD 0
#define BUZZER 10
#define SAFE_SWITCH 3
#define LED_R 9
#define LED_G 2
#define LED_B 6
#define P_TONE 200
#define N_TONE 1000
  /*
     INFO:
     Модуль вектора земного притяжения для GY-521 примерно равен 16700 (+-1000)
  */

class SystemLogger;
class OrintationUnit;
class String;

class BlipSystem {
private:
    // OrintationUnit* pImuUnit = new MpuOrintationUnit(AXIS_ROTATION, true);
    OrintationUnit* pImuUnit;
    SystemLogger* pLogger = new SystemLogger(this, LOG_DELTA);
    BlipState* pState = BlipState::getBlipEmptyState();
    IEventManager* eventManager;
    LinkedList<blip::Pair<BlipSub*, const String>> unsubscribedList;

    Servo* pServoX = new Servo;
    Servo* pServoZ = new Servo;

#ifdef USE_BMP388
    Adafruit_BMP3XX bmp; // I2C
//Adafruit_BMP3XX bmp(BMP_CS); // hardware SPI
//Adafruit_BMP3XX bmp(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);
#else
    Adafruit_BMP280 bmp; // I2C
                         //Adafruit_BMP280 bmp(BMP_CS); // hardware SPI
                         //Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);
#endif
    //SPIFlash flash(FLASH_SS, 0xEF30);

    long accX = 0;
    long accY = 0;
    long accZ = 0;
    double gyroX = 0;
    double gyroY = 0;
    double gyroZ = 0;
    double angularSpeedX = 0;
    double angularSpeedY = 0;
    double angularSpeedZ = 0;
    double height = 0;
    double zeroHeight = 0;

    double kP = 19.6, kI = 0.0, kD = 8.95, last_x_e = 0, last_z_e = 0, last_x_i = 0, last_z_i = 0,
        goal_height = 9000, seaAirPressure = 1013.25;
    unsigned long loopTimer = 0;

    unsigned int* pBuzzerImpulses = NULL, * pBuzzerTones = NULL;

    int buzzerN = 0;

    //CheckState
    int event_meter = 0, event_code = 0;

    //Indication
    bool buzzerState = false, ledState = false, ledColors[3];
    int buzzerTone = 0, buzzerDelta = 0, buzzerCount = 0, ledDelta = 0, ledCount = 0;
    unsigned long buzzerTimer = 0, ledTimer = 0;

    //Parachute
    unsigned long landingTimer = 0;

    void flushUnsubscribed();

public:
    BlipSystem();
    BlipSystem(OrintationUnit* orintationUnit, IEventManager* manager);

    void init();
    void execute();

    void checkState();

    void setIndication(int buzTone, unsigned long buzDelta = 0, int buzCount = -1);
    void setIndication(bool rgbCls[3], unsigned long rgbDelta = 0, int rgbCount = -1);
    void setIndication(int buzTone, unsigned long buzDelta, bool rgbCls[3], unsigned long rgbDelta, int buzCount = -1, int rgbCount = -1);
    void indicate();

    void thrusterStabilization(double dt);

    void waitingStart();

    void updatePosition();

    void subscribe(BlipSubscriber* sub, const String& type);
    void unsubscribe(BlipSubscriber* sub, const String& type);
    void notify(const String& type);

    void changeState(BlipState* newState, bool removeLast = false);
    BlipState* getState() { return pState; }

    long getAccX() { return accX; }
    long getAccY() { return accY; }
    long getAccZ() { return accZ; }

    double getGyroX() { return gyroX; }
    double getGyroY() { return gyroY; }
    double getGyroZ() { return gyroZ; }

    double getAngularSpeedX() { return angularSpeedX; }
    double getAngularSpeedY() { return angularSpeedY; }
    double getAngularSpeedZ() { return angularSpeedZ; }

    double getHeight() { return height; }
    double getZeroHeight() { return zeroHeight; }
    void setZeroHeight(double zeroHeight) { this->zeroHeight = zeroHeight; }

    Servo* getServoXPointer() { return pServoX; }
    Servo* getServoZPointer() { return pServoZ; }

    SystemLogger* getSystemLogger() { return pLogger; }

    OrintationUnit* getImuUnitPointer() { return pImuUnit; }
};

#endif
