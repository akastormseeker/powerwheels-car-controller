#pragma once

#include <Arduino.h>

#include "InputStream.h"

class SmoothedInputStream : public InputStream {
    public:
        SmoothedInputStream(InputStream *source);

        void loop(Timekeeper *tk);
        double getValue();

        InputStream *getSource();

        void setSmoothingFactor(double factor);
        double getSmoothingFactor();

    private:
        InputStream *_source;
        double _smoothingFactor;
        double _currentValue;
};