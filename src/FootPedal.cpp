#include "FootPedal.h"

FootPedal::FootPedal(int pin) {
    InputStream *stream;
    stream = this->_rawInput = new MillivoltsInputStream(pin);
    
    stream = this->_smoothedInput = new SmoothedInputStream(stream);
    this->_smoothedInput->setSmoothingFactor(0.1);

    stream = this->_scaledInput = new ScaledInputStream(stream);
    this->_scaledInput->setInputScale(0.0, 3300.0);
    this->_scaledInput->setOutputScale(-0.05, 1.05); // 5% deade zones at top and bottom
    this->_scaledInput->setClampOutput(false);

    stream = this->_clampedInput = new ClampedInputStream(stream);
    this->_clampedInput->setClampMax(1.0);
    this->_clampedInput->setClampMin(0.0);

    stream = this->_overrideInput = new InputStreamOverride(stream);
    this->_overrideInput->setOverrideValue(0.0);
    this->_overrideInput->setOverrideWeight(0.0);

    stream = this->_slopeLimitedInput = new SlopeLimitedInputStream(stream);
    this->_slopeLimitedInput->setMaxDownSlope(-10.0);
    this->_slopeLimitedInput->setMaxUpSlope(1.0);
    this->_slopeLimitedInput->setMinTimeSlice(0.0005);

    this->_lastStream = stream;
}

void FootPedal::setUpValue(double value) { this->_scaledInput->setInputScaleMin(value); }
double FootPedal::getUpValue() { return this->_scaledInput->getInputScaleMin(); }

void FootPedal::setDownValue(double value) { this->_scaledInput->setInputScaleMax(value); }
double FootPedal::getDownValue() { return this->_scaledInput->getInputScaleMax(); }

void FootPedal::setUpDeadZone(double value) { this->_scaledInput->setOutputScaleMin(-value); }
double FootPedal::getUpDeadZone() { return this->_scaledInput->getOutputScaleMin(); }

void FootPedal::setDownDeadZone(double value) { this->_scaledInput->setOutputScaleMax(1.0 + (1.0 - value)); }
double FootPedal::getDownDeadZone() { return this->_scaledInput->getOutputScaleMax(); }

void FootPedal::setMaxDownSlope(double value) { this->_slopeLimitedInput->setMaxDownSlope(value); }
double FootPedal::getMaxDownSlope() { return this->_slopeLimitedInput->getMaxDownSlope(); }

void FootPedal::setMaxUpSlope(double value) { this->_slopeLimitedInput->setMaxUpSlope(value); }
double FootPedal::getMaxUpSlope() { return this->_slopeLimitedInput->getMaxUpSlope(); }

void FootPedal::setMaxSlope(double value) { this->_slopeLimitedInput->setMaxSlope(value); }

void FootPedal::overridePedalValue(double value) {
    this->_overrideInput->setOverrideValue(value);
    this->_overrideInput->setOverrideWeight(1.0);
}

void FootPedal::cancelOverride() {
    this->_overrideInput->setOverrideWeight(0.0);
}

bool FootPedal::isOverridingInput() {
    return this->_overrideInput->isOverridingInput();
}

void FootPedal::loop(Timekeeper *tk) {
    this->_lastStream->loop(tk);
}

bool FootPedal::calibrate(int mode) {
    if(mode == FOOTPEDAL_CALIBRATE_UP) {
        double prevValue = this->_scaledInput->getInputScaleMin();
        double newValue = this->_smoothedInput->getValue();
        this->_scaledInput->setInputScaleMin(newValue);
        bool stable = (abs(newValue - prevValue) < 0.00001);
        return stable;
    }
    else if(mode == FOOTPEDAL_CALIBRATE_DOWN) {
        double prevValue = this->_scaledInput->getInputScaleMax();
        double newValue = this->_smoothedInput->getValue();
        this->_scaledInput->setInputScaleMax(newValue);
        bool stable = (abs(newValue - prevValue) < 0.00001);
        return stable;
    }
    return false;
}

double FootPedal::getPosition() { return this->_lastStream->getValue(); }
double FootPedal::getRawPosition() { return this->_rawInput->getValue(); }
double FootPedal::getSmoothedPosition() { return this->_smoothedInput->getValue(); }
double FootPedal::getScaledPosition() { return this->_scaledInput->getValue(); }
double FootPedal::getClampedPosition() { return this->_clampedInput->getValue(); }
double FootPedal::getSlopeLimitedPosition() { return this->_slopeLimitedInput->getValue(); }

double FootPedal::getFrameSlope() { return this->_slopeLimitedInput->getInstantSlope(); }
double FootPedal::getSmoothedSlope() { return this->_slopeLimitedInput->getSmoothedSlope(); }