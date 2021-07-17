#pragma once
#include <SD.h>



class Logger {

public:

    void logData();

    void logMessage(String message);

};

extern Logger LOGGER;