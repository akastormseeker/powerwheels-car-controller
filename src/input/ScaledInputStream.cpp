#include "ScaledInputStream.h"

ScaledInputStream::ScaledInputStream(InputStream *source) {
    this->_source = source;
    this->_inputMax = 1.0;
    this->_inputMin = 0.0;
    this->_outputMax = 1.0;
    this->_outputMin = 0.0;
    this->_clampOutput = false;
}

void ScaledInputStream::loop(Timekeeper *tk) {
    this->_source->loop(tk);
}

void ScaledInputStream::setInputScale(double minVal, double maxVal) {
    this->_inputMin = minVal;
    this->_inputMax = maxVal;
    this->_inputDelta = this->_inputMax - this->_inputMin;
}

void ScaledInputStream::setInputScaleMin(double minVal) {
    this->_inputMin = minVal;
    this->_inputDelta = this->_inputMax - this->_inputMin;
}
double ScaledInputStream::getInputScaleMin() { return this->_inputMin; }

void ScaledInputStream::setInputScaleMax(double maxVal) {
    this->_inputMax = maxVal;
    this->_inputDelta = this->_inputMax - this->_inputMin;
}
double ScaledInputStream::getInputScaleMax() { return this->_inputMax; }

void ScaledInputStream::setOutputScale(double minVal, double maxVal) {
    this->_outputMin = minVal;
    this->_outputMax = maxVal;
    this->_outputDelta = this->_outputMax - this->_outputMin;
    this->_realOutputMax = max(minVal, maxVal);
    this->_realOutputMin = min(minVal, maxVal);
}

void ScaledInputStream::setOutputScaleMin(double minVal) {
    this->_outputMin = minVal;
    this->_outputDelta = this->_outputMax - this->_outputMin;
    this->_realOutputMax = max(minVal, this->_outputMax);
    this->_realOutputMin = min(minVal, this->_outputMax);
}
double ScaledInputStream::getOutputScaleMin() { return this->_outputMin; }

void ScaledInputStream::setOutputScaleMax(double maxVal) {
    this->_outputMax = maxVal;
    this->_outputDelta = this->_outputMax - this->_outputMin;
    this->_realOutputMax = max(this->_outputMin, maxVal);
    this->_realOutputMin = min(this->_outputMin, maxVal);
}
double ScaledInputStream::getOutputScaleMax() { return this->_outputMax; }

void ScaledInputStream::setClampOutput(bool clamp) {
    this->_clampOutput = clamp;
}
bool ScaledInputStream::isOutputClamped() { return this->_clampOutput; }

double ScaledInputStream::getValue() {
    double val = this->_source->getValue();
    val = (val - this->_inputMin) / this->_inputDelta;
    val = (val * this->_outputDelta) + this->_outputMin;
    if(this->_clampOutput) {
        if(val > this->_realOutputMax) val = this->_realOutputMax;
        if(val < this->_realOutputMin) val = this->_realOutputMin;
    }
    return isnan(val) ? 0.0 : val;
}