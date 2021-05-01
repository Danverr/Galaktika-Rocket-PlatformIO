#pragma once
#include <Arduino.h>

#define RF_RESPONSE_SIZE 2
#define RF_REQUEST_SIZE 3
#define RF_MAX_RESPONSE_ID 120
#define RF_ESCAPE_MARKER 0x7D
#define RF_START_MARKER 0x12
#define RF_VAR_SIZE 2
#define RF_PRECISION 10

class RequestHandler {

public:

    RequestHandler();

    void read(byte b);

    const byte* getResponse();

private:

    int currentByte = RF_REQUEST_SIZE;
    bool isEscaped = false;
    byte buff[RF_REQUEST_SIZE];
    int lastRequestId = -1;
    byte response[RF_RESPONSE_SIZE];

};

extern RequestHandler requestHandler;