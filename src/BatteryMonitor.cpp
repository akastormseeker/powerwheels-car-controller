#include "BatteryMonitor.h"


BatteryMonitor::BatteryMonitor(int pin) {
    InputStream *stream;

    stream = this->_rawInput = new MillivoltsInputStream(pin);

    stream = this->_smoothedInput = new SmoothedInputStream(stream);
    this->_smoothedInput->setSmoothingFactor(0.01);

    stream = this->_voltsLevel = new ScaledInputStream(stream);
    this->_voltsLevel->setInputScale(0.0, 3300.0);
    this->_voltsLevel->setOutputScale(0.0, 18.8);
    this->_voltsLevel->setClampOutput(false);

    stream = this->_percentLevel = new ScaledInputStream(stream);
    this->_percentLevel->setInputScale(10.0, 12.3);
    this->_percentLevel->setOutputScale(0.0, 100.0);
    this->_percentLevel->setClampOutput(true);

    this->_lastVoltageReading = 0;
    this->_isStable = false;
}

void BatteryMonitor::loop(Timekeeper *tk) {
    this->_percentLevel->loop(tk);
    if(!this->_isStable) {
        double newVoltage = this->getBatteryVoltage();
        if(abs(newVoltage - this->_lastVoltageReading) < 0.001) {
            this->_isStable = true;
            this->_smoothedInput->setSmoothingFactor(0.0001);
        }
        this->_lastVoltageReading = newVoltage;
    }
}

bool BatteryMonitor::isStable() { return this->_isStable; }

double BatteryMonitor::getRawReading() { return this->_smoothedInput->getValue(); }
double BatteryMonitor::getBatteryVoltage() { return this->_voltsLevel->getValue(); }
double BatteryMonitor::getBatteryPercent() { return this->_percentLevel->getValue(); }

bool BatteryMonitor::isBatteryDead() { return this->getBatteryPercent() < 1.0; };