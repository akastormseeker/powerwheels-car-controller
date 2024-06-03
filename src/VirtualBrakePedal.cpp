#include "VirtualBrakePedal.h"

double clamp(double value, double min, double max) {
    if(value < min) value = min;
    if(value > max) value = max;

    return value;
}

VirtualBrakePedal::VirtualBrakePedal() {
    InputStream *stream;

    stream = _rawInput = new ValueInputStream(0.0);

    stream = this->_clampedInput = new ClampedInputStream(stream);
    this->_clampedInput->setClampMax(1.0);
    this->_clampedInput->setClampMin(0.0);

    stream = this->_slopeLimitedInput = new SlopeLimitedInputStream(stream);
    this->_slopeLimitedInput->setMaxDownSlope(-10.0);
    this->_slopeLimitedInput->setMaxUpSlope(1.0);
    this->_slopeLimitedInput->setMinTimeSlice(0.0005);

    this->_lastInput = stream;
}

void VirtualBrakePedal::loop(Timekeeper *tk) {
    this->_lastInput->loop(tk);
}

void VirtualBrakePedal::setMinActiveLevel(double level) { this->_minActiveLevel = clamp(level, 0.0, 1.0); }

void VirtualBrakePedal::setPedalPosition(double position) { this->_rawInput->setValue(clamp(position, 0.0, 1.0)); }
void VirtualBrakePedal::activate() { this->setPedalPosition(1.0); }
void VirtualBrakePedal::deactivate() { this->setPedalPosition(0.0); }

double VirtualBrakePedal::getPosition() { return this->_lastInput->getValue(); }
double VirtualBrakePedal::getRawPosition() { return this->_rawInput->getValue(); }
double VirtualBrakePedal::getClampedPosition() { return this->_clampedInput->getValue(); }
double VirtualBrakePedal::getSlopeLimitedPosition() { return this->_slopeLimitedInput->getValue(); }
bool VirtualBrakePedal::isActivated() { return this->_lastInput->getValue() < this->_minActiveLevel; }