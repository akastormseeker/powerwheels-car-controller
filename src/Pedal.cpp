#include <Arduino.h>

#include "Pedal.h"
#include "Timekeeper.h"

Pedal::Pedal(int pin) {
    this->_pin = pin;
    this->_upValue = 0;
    this->_downValue = 3300; // 3.3v
    this->_upDeadZone = 0.0; // 0.0 to upDeadZone
    this->_downDeadZone = 1.0; // downDeadZone to 1.0
    this->_rawValue = 0;
    this->_rawPercent = 0.0;
    this->_smoothedPercent = 0.0;
    this->_filteredPercent = 0.0;
    this->_smoothingFactor = 0.1;
    this->_isOverridingRead = false;
}

uint32_t Pedal::readRaw() {
    return analogReadMilliVolts(this->_pin);
}

long clamp(long value, long min, long max) {
    if(value < min) value = min;
    if(value > max) value = max;
    return value;
}

void Pedal::loop(Timekeeper *timekp) {
    this->_rawValue = this->readRaw();
    if(this->_upValue == this->_downValue) {
        this->_rawPercent = 0.0;
    }
    else {
        this->_rawPercent = (clamp(map(this->_rawValue, this->_upValue, this->_downValue, 0, 100000), 0, 100000) / 100000.0);
    }
    if(this->_isOverridingRead) {
        this->_rawPercent = this->_rawPercentOverrideValue;
    }
    this->_smoothedPercent = (this->_smoothedPercent * (1.0 - this->_smoothingFactor)) + (this->_rawPercent * this->_smoothingFactor);
    
    double targetFilteredPercent = 0.0;
    if(this->_smoothedPercent <= this->_upDeadZone) {
        targetFilteredPercent = 0.0;
    }
    else if(this->_smoothedPercent >= this->_downDeadZone) {
        targetFilteredPercent = 1.0;
    }
    else {
        targetFilteredPercent = (this->_smoothedPercent - this->_upDeadZone) / (this->_downDeadZone - this->_upDeadZone);
    }

    double lastFilteredPercent = this->_filteredPercent;
    double filterPercentDelta = targetFilteredPercent - lastFilteredPercent;
    double maxFrameChange = 0;
    if(filterPercentDelta >= 0) {
        maxFrameChange = this->_maxUpSlope * timekp->frameTime;
        this->_filteredPercent += min(filterPercentDelta, maxFrameChange);
    }
    else {
        maxFrameChange = this->_maxDownSlope * timekp->frameTime;
        this->_filteredPercent += max(filterPercentDelta, maxFrameChange);
    }


    this->_frameSlope = (this->_filteredPercent - lastFilteredPercent) / timekp->frameTime;
    this->_filteredSlope = (this->_filteredSlope * (1.0 - this->_smoothingFactor)) + (this->_frameSlope * this->_smoothingFactor);
    
}

bool Pedal::calibrate(int mode) {
    if(mode == PEDAL_CALIBRATE_UP) {
        this->_upValue = (uint32_t)((this->_upValue * (1.0 - this->_smoothingFactor)) + (this->_rawValue * this->_smoothingFactor));
        bool stable = this->_upValue == this->_lastCalibrationValue;
        this->_lastCalibrationValue = this->_upValue;
        return stable;
    }
    else if(mode == PEDAL_CALIBRATE_DOWN) {
        this->_downValue = (uint32_t)((this->_downValue * (1.0 - this->_smoothingFactor)) + (this->_rawValue * this->_smoothingFactor));
        bool stable = this->_downValue == this->_lastCalibrationValue;
        this->_lastCalibrationValue = this->_downValue;
        return stable;
    }
    return false;
}

void Pedal::overrideReadPercent(double percent) {
    this->_isOverridingRead = true;
    this->_rawPercentOverrideValue = percent;
}

void Pedal::cancelReadOverride() {
    this->_isOverridingRead = false;
}

bool Pedal::isOverridingRead() {
    return this->_isOverridingRead;
}

uint32_t Pedal::getRawValue() {
    return this->_rawValue;
}

double Pedal::getRawPosition() {
    return this->_rawPercent;
}

double Pedal::getSmoothedPosition() {
    return this->_smoothedPercent;
}

double Pedal::getPosition() {
    return this->_filteredPercent;
}


void Pedal::setUpValue(uint32_t value) {
    this->_upValue = value;
}

void Pedal::setDownValue(uint32_t value) {
    this->_downValue = value;
}

uint32_t Pedal::getUpValue() {
    return this->_upValue;
}

uint32_t Pedal::getDownValue() {
    return this->_downValue;
}

void Pedal::setUpDeadZone(double deadZone) {
    this->_upDeadZone = deadZone;
}

void Pedal::setDownDeadZone(double deadZone) {
    this->_downDeadZone = deadZone;
}

double Pedal::getUpDeadZone() {
    return this->_upDeadZone;
}

double Pedal::getDownDeadZone() {
    return this->_downDeadZone;
}

double Pedal::getFrameSlope() {
    return this->_frameSlope;
}

double Pedal::getSlope() {
    return this->_filteredSlope;
}

void Pedal::setMaxDownSlope(double maxSlope) {
    this->_maxDownSlope = (maxSlope > 0) ? -maxSlope : maxSlope;
}

void Pedal::setMaxUpSlope(double maxSlope) {
    this->_maxUpSlope = (maxSlope < 0) ? -maxSlope : maxSlope;
}

void Pedal::setMaxSlope(double maxSlope) {
    this->setMaxUpSlope(maxSlope);
    this->setMaxDownSlope(maxSlope);
}

double Pedal::getMaxDownSlope() {
    return this->_maxDownSlope;
}

double Pedal::getMaxUpSlope() {
    return this->_maxUpSlope;
}