#include "ClampedInputStream.h"

ClampedInputStream::ClampedInputStream(InputStream *source) {
    this->_source = source;
    this->_clampMin = 0.0;
    this->_clampMax = 1.0;
}

void ClampedInputStream::setClampMin(double value) {
    this->_clampMin = value;
}

void ClampedInputStream::setClampMax(double value) {
    this->_clampMax = value;
}

void ClampedInputStream::loop(Timekeeper *tk) {
    this->_source->loop(tk);
}

double ClampedInputStream::getValue() {
    double value = this->_source->getValue();
    if(value > this->_clampMax) value = this->_clampMax;
    if(value < this->_clampMin) value = this->_clampMin;
    return value;
}