#ifndef _SYSTEM_LOGGER_BLIP_H_
#define _SYSTEM_LOGGER_BLIP_H_

#include <SD.h>
#include "RequestHandler.h"
//#include "BlipSystem.h"

enum FlightStages {
    ABORT_STAGE = -1,
    WAITING_LAUNCH_STAGE,
    POWERED_FLIGHT_STAGE,
    APOGEE_STAGE,
    PARACHUTE_DEPLOYMENT_STAGE,
    LANDING_STAGE,
};

class BlipSystem;

class SystemLogger {
private:
    BlipSystem* pBlipSystem;
    File file;
    const char* fileName;
    unsigned long startTimer = 0, logDelta, logTimer = 0;
    FlightStages currentStage = WAITING_LAUNCH_STAGE;

    void begin(int time);
    void log(byte bytes[], int n);
    void log(long val);
    void log(int val);
    void log(double val);

public:
    SystemLogger(BlipSystem* pBS, unsigned long _logDelta = 50, char* _fileName = "data.txt");

    bool start();

    void logInfo();

    void logEvent(String eventName);

    void setStage(FlightStages stage);

    void finish();
};

#endif
