#pragma once

#include <Arduino.h>
#include "input/InputStream.h"
#include "input/ValueInputStream.h"
#include "input/ClampedInputStream.h"
#include "input/SlopeLimitedInputStream.h"

class VirtualBrakePedal {
    public:
        VirtualBrakePedal();

        void loop(Timekeeper *tk);
        void setMinActiveLevel(double level);

        void setPedalPosition(double position);
        void activate();
        void deactivate();

        double getPosition();
        double getRawPosition();
        double getClampedPosition();
        double getSlopeLimitedPosition();
        bool isActivated();

    private:
        ValueInputStream *_rawInput;
        ClampedInputStream *_clampedInput;
        SlopeLimitedInputStream *_slopeLimitedInput;
        InputStream *_lastInput;

        double _minActiveLevel;

};