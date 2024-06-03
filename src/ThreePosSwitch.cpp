#include <Arduino.h>

#include "ThreePosSwitch.h"

ThreePosSwitch::ThreePosSwitch(int pin1, int pin2, int inputMode, int activeState) {
    this->_pin1 = pin1;
    this->_pin2 = pin2;
    this->_activeState = activeState;

    pinMode(this->_pin1, inputMode);
    pinMode(this->_pin2, inputMode);

    this->_pos = SWITCH_POS_OFF;

    this->_onChangedCallback = NULL;
    this->_debounceDelay = 50;
    this->_pin1Pause = millis();
    this->_pin2Pause = millis();
}

void ThreePosSwitch::setOnChangedCallback(void (*callback)(ThreePosSwitch* sender, int newState)) {
    this->_onChangedCallback = callback;
}

void ThreePosSwitch::setDebounceDelay(unsigned int delay) {
    this->_debounceDelay = delay;
}

void ThreePosSwitch::loop() {
    bool somethingChanged = false;

    if(millis() >= this->_pin1Pause) {
        int p1 = digitalRead(this->_pin1);
        if(p1 != this->_lastPin1Val) {
            this->_lastPin1Val = p1;
            somethingChanged = true;
            this->_pin1Pause = millis() + this->_debounceDelay;
        }
    }
    if(millis() >= this->_pin2Pause) {
        int p2 = digitalRead(this->_pin2);
        if(p2 != this->_lastPin2Val) {
            this->_lastPin2Val = p2;
            somethingChanged = true;
            this->_pin2Pause = millis() + this->_debounceDelay;
        }
    }
    
    if(somethingChanged) {
        if(this->_lastPin1Val == this->_lastPin2Val) {
            this->_pos = SWITCH_POS_OFF;
        }
        else if(this->_lastPin1Val == this->_activeState) {
            this->_pos = SWITCH_POS_A;
        }
        else if(this->_lastPin2Val == this->_activeState) {
            this->_pos = SWITCH_POS_B;
        }

        if(this->_onChangedCallback != NULL) {
            this->_onChangedCallback(this, this->_pos);
        }
    }
}

int ThreePosSwitch::getPosition() {
    return this->_pos;
}

bool ThreePosSwitch::isOff() {
    return this->_pos == SWITCH_POS_OFF;
}

bool ThreePosSwitch::isPosA() {
    return this->_pos == SWITCH_POS_A;
}

bool ThreePosSwitch::isPosB() {
    return this->_pos == SWITCH_POS_B;
}