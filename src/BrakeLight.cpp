#include <Arduino.h>

#include "BrakeLight.h"

BrakeLight::BrakeLight(int pin, bool invertedOutput, long holdTime, long fadeTime) {
    this->_pin = pin;
    this->_invertedOutput = invertedOutput;
    this->_holdTime = holdTime;
    this->_fadeTime = fadeTime;
    this->_onLevel = 0xff;
    this->_offLevel = 0;

    pinMode(this->_pin, OUTPUT);
    this->_setLevel(0);
}

void BrakeLight::setLevels(int on, int off) {
    if(this->_currentLevel == this->_offLevel) {
        this->_setLevel(off);
    }
    this->_onLevel = on;
    this->_offLevel = off;
}

void BrakeLight::_setLevel(int level) {
    int outputLevel = (this->_invertedOutput ? 0xff - level : level);
    analogWrite(this->_pin, outputLevel);
    this->_currentLevel = level;
}

void BrakeLight::loop() {
    if(this->_currentLevel > this->_offLevel) {
        unsigned long runtime = (millis() - this->_lastActivation);
        if(runtime < this->_holdTime) {
            if(this->_currentLevel != this->_onLevel) {
                this->_setLevel(this->_onLevel);
            }
        }
        else if(runtime < (this->_holdTime + this->_fadeTime)) {
            unsigned long fadeTime = (runtime - this->_holdTime);
            int fadeLevel = map(fadeTime, 0, this->_fadeTime, this->_onLevel, this->_offLevel);
            if(this->_currentLevel != fadeLevel) {
                this->_setLevel(fadeLevel);
            }
        }
        else {
            this->_setLevel(this->_offLevel);
        }
    } 
    else if(this->_currentLevel != this->_offLevel) {
        this->_setLevel(this->_offLevel);
    }
}

void BrakeLight::activate() {
    this->_lastActivation = millis();
    this->_setLevel(this->_onLevel);
}

void BrakeLight::deactivate() {
    this->_lastActivation = 0;
    this->_setLevel(this->_offLevel);
}