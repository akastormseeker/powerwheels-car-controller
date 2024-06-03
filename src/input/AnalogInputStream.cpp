#include "AnalogInputStream.h"

AnalogInputStream::AnalogInputStream(int pin) {
    this->_pin = pin;
    pinMode(this->_pin, INPUT);
}

void AnalogInputStream::loop(Timekeeper *tk) {}

double AnalogInputStream::getValue() {
    return (double)analogRead(this->_pin);
}