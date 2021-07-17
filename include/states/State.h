#pragma once
#include "ImportSensors.h"
#include "Logger.h"



class State {

public:

    // Тут происходит вся работа
    virtual void execute() = 0;

    // Метод для перехода к новой стадии
    void changeState(State* nextState);

private:

    // Вызывается в начале новой стадии
    void onStart() {};

    // Вызывается по окончанию стадии
    void onEnd() {};

};

extern State* ROCKET_STATE;