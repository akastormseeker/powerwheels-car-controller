#include "SmoothedInputStream.h"

SmoothedInputStream::SmoothedInputStream(InputStream *source) {
    this->_source = source;
    this->_smoothingFactor = 0.1;
    this->_currentValue = this->_source->getValue();
}

void SmoothedInputStream::loop(Timekeeper *tk) {
    this->_source->loop(tk);
    
    double newValue = this->_source->getValue();
    this->_currentValue = (1.0 - this->_smoothingFactor) * this->_currentValue;
    this->_currentValue += this->_smoothingFactor * newValue;
}

double SmoothedInputStream::getValue() { return this->_currentValue; }
InputStream* SmoothedInputStream::getSource() { return this->_source; }
double SmoothedInputStream::getSmoothingFactor() { return this->_smoothingFactor; }
void SmoothedInputStream::setSmoothingFactor(double factor) { this->_smoothingFactor = factor; }