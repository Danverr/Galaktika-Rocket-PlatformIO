#include "states/InitState.h"



void InitState::execute() {
    Sensor* sensors[] = { ALTIMETER, GPS, IMU };

    while (true) {
        bool initSucceeded = true;
        String log = "INITIALIZATION STARTED";

        for (const auto& sensor : sensors) {
            log += "\n" + sensor->getSensorType() + " " + sensor->getSensorName() + ": ";

            if (sensor->begin()) {
                log += "ERROR";
                initSucceeded = false;
            } else {
                log += "OK";
            }
        }

        LOGGER.logMessage(log);

        if (initSucceeded) {
            break;
        } else {
            delay(5000);
        }
    }

    changeState(new WaitingStartState());
}