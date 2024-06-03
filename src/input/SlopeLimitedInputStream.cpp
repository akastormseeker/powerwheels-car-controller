#include "SlopeLimitedInputStream.h"

SlopeLimitedInputStream::SlopeLimitedInputStream(InputStream *source) {
    this->_source = source;
    this->_currentValue = 0.0;

    this->_slope = new ValueInputStream(0.0);
    this->_smoothedSlope = new SmoothedInputStream(this->_slope);
    this->_smoothedSlope->setSmoothingFactor(0.01);

    this->setMaxSlope(1.0);
    this->_minTimeSlice = 0.001;
    this->_lastRunTime = -1;
};

void SlopeLimitedInputStream::setMaxUpSlope(double maxSlope) {
    this->_maxUpSlope = maxSlope;
}

void SlopeLimitedInputStream::setMaxDownSlope(double maxSlope) {
    this->_maxDownSlope = maxSlope;
}

void SlopeLimitedInputStream::setMaxSlope(double maxUpSlope, double maxDownSlope) {
    this->_maxUpSlope = maxUpSlope;
    this->_maxDownSlope = maxDownSlope;
}

void SlopeLimitedInputStream::setMaxSlope(double maxSlope) {
    this->_maxUpSlope = maxSlope;
    this->_maxDownSlope = -maxSlope;
}

double SlopeLimitedInputStream::getMaxUpSlope() { return this->_maxUpSlope; }
double SlopeLimitedInputStream::getMaxDownSlope() { return this->_maxDownSlope; }

void SlopeLimitedInputStream::setMinTimeSlice(double minTimeSlice) {
    this->_minTimeSlice = minTimeSlice;
}

void SlopeLimitedInputStream::loop(Timekeeper *tk) {
    if(this->_lastRunTime < 0) this->_lastRunTime = tk->runTime;
    if((tk->runTime - this->_lastRunTime) < this->_minTimeSlice) return;

    double frameTime = tk->runTime - this->_lastRunTime;
    this->_lastRunTime = tk->runTime;


    this->_source->loop(tk);

    double newValue = this->_source->getValue();
    double valueDelta = newValue - this->_currentValue;
    double maxValueDelta;
    if(valueDelta >= 0.0) {
        maxValueDelta = this->_maxUpSlope * frameTime;
        valueDelta = min(valueDelta, maxValueDelta);
    }
    else {
        maxValueDelta = this->_maxDownSlope * frameTime;
        valueDelta = max(valueDelta, maxValueDelta);
    }
    this->_currentValue += valueDelta;

    this->_slope->setValue(valueDelta / frameTime);
    this->_smoothedSlope->loop(tk);
}

double SlopeLimitedInputStream::getValue() { return this->_currentValue; }
double SlopeLimitedInputStream::getInstantSlope() { return this->_slope->getValue(); }
double SlopeLimitedInputStream::getSmoothedSlope() { return this->_smoothedSlope->getValue(); }