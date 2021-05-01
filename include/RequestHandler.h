#pragma once
#include <Arduino.h>
#include "config.h"

class BlipSystem;

class RequestHandler {

public:

    RequestHandler();
    RequestHandler(BlipSystem* system);

    void read(byte b);

    const byte* getResponse();

    void setBlipSystem(BlipSystem* system) { this->system = system; }
    BlipSystem* getBlipSystem() { return system; }

private:
    BlipSystem* system = nullptr;

    int currentByte = RF_REQUEST_SIZE;
    bool isEscaped = false;
    byte buff[RF_REQUEST_SIZE];
    int lastRequestId = -1;
    byte response[RF_RESPONSE_SIZE];

};

// extern RequestHandler requestHandler;
