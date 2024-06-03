#include "MillivoltsInputStream.h"

MillivoltsInputStream::MillivoltsInputStream(int pin) {
    this->_pin = pin;
    pinMode(this->_pin, INPUT);
}

void MillivoltsInputStream::loop(Timekeeper *tk) {}

double MillivoltsInputStream::getValue() {
    return (double)analogReadMilliVolts(this->_pin);
}