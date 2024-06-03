#include "ValueInputStream.h"

ValueInputStream::ValueInputStream(double initialValue) {
    this->_value = initialValue;
}

ValueInputStream::ValueInputStream() : ValueInputStream(0.0) {}

void ValueInputStream::loop(Timekeeper *tk) {}

double ValueInputStream::getValue() { return this->_value; }
void ValueInputStream::setValue(double value) { this->_value = value; }