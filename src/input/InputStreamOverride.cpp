#include "InputStreamOverride.h"

InputStreamOverride::InputStreamOverride(InputStream *source) {
    this->_source = source;
    this->_overrideValue = 0.0;
    this->_weight = 0.0;
}

void InputStreamOverride::setOverrideValue(double value) { this->_overrideValue = value; }
double InputStreamOverride::getOverrideValue() { return this->_overrideValue; }
void InputStreamOverride::setOverrideWeight(double weight) { this->_weight = min(max(0.0, weight), 1.0); }
double InputStreamOverride::getOverrideWeight() { return this->_weight; }
bool InputStreamOverride::isOverridingInput() { return this->_weight > 0.0; }

void InputStreamOverride::loop(Timekeeper *tk) { this->_source->loop(tk); }

double InputStreamOverride::getValue() {
    double val = this->_source->getValue() * (1.0 - this->_weight);
    val += this->_overrideValue * this->_weight;
    return val;
}