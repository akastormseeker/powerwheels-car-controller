#pragma once

#include <Arduino.h>
#include "InputStream.h"

class ClampedInputStream : public InputStream {
    public:
        ClampedInputStream(InputStream *source);

        void setClampMin(double value);
        void setClampMax(double value);

        void loop(Timekeeper *tk);
        double getValue();

    private:
        InputStream *_source;
        double _clampMax;
        double _clampMin;
};