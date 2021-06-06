#pragma once
#include <Arduino.h>
#include "config.h"

class BlipSystem;
class BlipState;

#define RH_CALIBRATION_CODE 5
#define RH_LAUNCH_CODE 6
#define RH_EMERGENCY_PARACHUTE_CODE 7

class RequestHandler {

public:

    RequestHandler();
    RequestHandler(BlipSystem* system);

    void read(byte b);

    const byte* getResponse();

    void setBlipSystem(BlipSystem* system) { this->system = system; }
    BlipSystem* getBlipSystem() { return system; }

    BlipState* getCalibrationState() { return calibrationState; }
    BlipState* getLaunchState() { return launchState; }
    BlipState* getEmergencyParachuteState() { return emergencyParachuteState; }

    void setCalibrationState(BlipState* state) { calibrationState = state; }
    void setLaunchState(BlipState* state) { launchState = state; }
    void setEmergencyParachuteState(BlipState* state) { emergencyParachuteState = state; }

private:
    BlipSystem* system = nullptr;

    int currentByte = RF_REQUEST_SIZE;
    bool isEscaped = false;
    byte buff[RF_REQUEST_SIZE];
    int lastRequestId = -1;
    byte response[RF_RESPONSE_SIZE];

    BlipState* calibrationState;
    BlipState* launchState;
    BlipState* emergencyParachuteState;

};

// extern RequestHandler requestHandler;
