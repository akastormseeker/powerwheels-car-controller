#pragma once

#include <Arduino.h>
#include "InputStream.h"

class ScaledInputStream : public InputStream {
    public:
        ScaledInputStream(InputStream *source);

        void setInputScale(double min, double max);
        void setInputScaleMin(double min);
        double getInputScaleMin();
        void setInputScaleMax(double max);
        double getInputScaleMax();
        void setOutputScale(double min, double max);
        void setOutputScaleMin(double min);
        double getOutputScaleMin();
        void setOutputScaleMax(double max);
        double getOutputScaleMax();
        void setClampOutput(bool clamp);
        bool isOutputClamped();

        void loop(Timekeeper *tk);
        double getValue();

    private:
        InputStream *_source;
        double _inputMax;
        double _inputMin;
        double _inputDelta;
        double _outputMax;
        double _outputMin;
        double _outputDelta;
        double _realOutputMax;
        double _realOutputMin;
        bool _clampOutput;
};