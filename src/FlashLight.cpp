#include <Arduino.h>

#include "FlashLight.h"

FlashLight::FlashLight(int pin) {
    this->_pin = pin;
    pinMode(this->_pin, OUTPUT);
    this->setOff();
}

void FlashLight::loop() {
    if(this->_isRunning) {
        unsigned long currentTime = (millis() - this->_startTime);
        unsigned long blinkTime = this->_onTime + this->_offTime;
        unsigned long loopTime = (blinkTime * this->_blinkCount) + this->_delayTime;

        if(loopTime == 0 && this->_loopCount <= 0) {
            // infinite loop of nothing - just stop
            this->_isRunning = false;
            return;
        }
        int loops = 0;
        if(loopTime > 0) {
            loops = currentTime / loopTime;
            currentTime = currentTime % loopTime;
        }
        else {
            loops = this->_loopCount;
        }
        
        if(this->_loopCount > 0) {
            if(loops >= this->_loopCount) {
                this->_isRunning = false;
                switch(this->_endMode) {
                    case FLASHLIGHT_END_OFF: this->_setOff(); break;
                    case FLASHLIGHT_END_ON: this->_setOn(); break;
                }
                return;
            }
        }
        else {
            if(loops > 0) {
                this->_startTime += (loops * loopTime);
            }
        }

        if(currentTime >= (blinkTime * this->_blinkCount)) {
            // in post-blink delay
            if(this->_currentLevel != LOW) {
                this->_setOff();
            }
            return;
        }
        currentTime = currentTime % blinkTime;
        if(currentTime < this->_onTime) {
            if(this->_currentLevel != HIGH) {
                this->_setOn();
            }
            return;
        }
        else {
            if(this->_currentLevel != LOW) {
                this->_setOff();
            }
        }
    }    
}

void FlashLight::setOn() {
    this->_blinkCount = 0;
    this->_onTime = 0;
    this->_offTime = 0;
    this->_loopCount = 1;
    this->_delayTime = 0;
    this->_endMode = FLASHLIGHT_END_ON;
    this->_startTime = millis();
    this->_isRunning = true;
    this->_setOn();
}

void FlashLight::setOff() {
    this->_blinkCount = 0;
    this->_onTime = 0;
    this->_offTime = 0;
    this->_loopCount = 1;
    this->_delayTime = 0;
    this->_endMode = FLASHLIGHT_END_OFF;   
    this->_startTime = millis();
    this->_isRunning = true;
    this->_setOff();
}

void FlashLight::setBlink(int onCount, unsigned long onTimeMs, unsigned long offTimeMs, unsigned long delayTimeMs, int endMode, int loopCount) {
    this->_blinkCount = onCount;
    this->_onTime = onTimeMs;
    this->_offTime = offTimeMs;
    this->_delayTime = delayTimeMs;
    this->_endMode = endMode;
    this->_loopCount = loopCount;
    this->_startTime = millis();
    this->_isRunning = true;
}

void FlashLight::setBlink(unsigned long onTimeMs, unsigned long offTimeMs, int endMode, int loopCount) {
    this->setBlink(0, onTimeMs, offTimeMs, 0, endMode, loopCount);
}

void FlashLight::_setLevel(int level) {
    this->_currentLevel = level;
    analogWrite(this->_pin, level);
}

void FlashLight::_setOn() {
    this->_currentLevel = HIGH;
    digitalWrite(this->_pin, HIGH);
}

void FlashLight::_setOff() {
    this->_currentLevel = LOW;
    digitalWrite(this->_pin, LOW);
}