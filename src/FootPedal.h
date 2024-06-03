#pragma once

#include <Arduino.h>
#include "Timekeeper.h"
#include "input/MillivoltsInputStream.h"
#include "input/ScaledInputStream.h"
#include "input/ClampedInputStream.h"
#include "input/SmoothedInputStream.h"
#include "input/SlopeLimitedInputStream.h"
#include "input/InputStreamOverride.h"

#define FOOTPEDAL_CALIBRATE_UP    0
#define FOOTPEDAL_CALIBRATE_DOWN  1

class FootPedal {
    public:
        FootPedal(int pin);

        void setUpValue(double value);
        double getUpValue();
        void setDownValue(double value);
        double getDownValue();
        
        void setUpDeadZone(double value);
        double getUpDeadZone();
        void setDownDeadZone(double value);
        double getDownDeadZone();

        void setMaxDownSlope(double value);
        double getMaxDownSlope();
        void setMaxUpSlope(double value);
        double getMaxUpSlope();
        void setMaxSlope(double value);

        void overridePedalValue(double value);
        void cancelOverride();
        bool isOverridingInput();

        void loop(Timekeeper *tk);
        bool calibrate(int mode);

        double getPosition();

        double getRawPosition();
        double getOverridePosition();
        double getSmoothedPosition();
        double getScaledPosition();
        double getClampedPosition();
        double getSlopeLimitedPosition();

        double getFrameSlope();
        double getSmoothedSlope();

    private:
        MillivoltsInputStream *_rawInput;
        InputStreamOverride *_overrideInput;
        SmoothedInputStream *_smoothedInput;
        ScaledInputStream *_scaledInput;
        ClampedInputStream *_clampedInput;
        SlopeLimitedInputStream *_slopeLimitedInput;
        InputStream *_lastStream;
};