#include "states/State.h"

void State::changeState(State* nextState) {
    onEnd();
    ROCKET_STATE = nextState;
    ROCKET_STATE->onStart();
}