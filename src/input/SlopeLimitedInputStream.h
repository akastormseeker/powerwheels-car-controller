#pragma once

#include <Arduino.h>
#include "InputStream.h"
#include "ValueInputStream.h"
#include "SmoothedInputStream.h"

class SlopeLimitedInputStream : public InputStream {
    public:
        SlopeLimitedInputStream(InputStream *source);

        void setMaxUpSlope(double maxSlope);
        void setMaxDownSlope(double maxSlope);
        void setMaxSlope(double maxSlope);
        void setMaxSlope(double maxUpSlope, double maxDownSlope);
        double getMaxUpSlope();
        double getMaxDownSlope();

        void setMinTimeSlice(double minTimeSlice);

        void loop(Timekeeper *tk);
        double getValue();

        double getInstantSlope();
        double getSmoothedSlope();


    private:
        InputStream *_source;
        double _maxUpSlope;
        double _maxDownSlope;
        double _lastRunTime;

        double _currentValue;

        ValueInputStream *_slope;
        SmoothedInputStream *_smoothedSlope;

        double _minTimeSlice;
};