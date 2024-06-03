#pragma once

#include <Arduino.h>
#include "Timekeeper.h"
#include "input/MillivoltsInputStream.h"
#include "input/SmoothedInputStream.h"
#include "input/ScaledInputStream.h"

#define BATTERY_STABILIZE_TIME  1000

class BatteryMonitor {
    public:
        BatteryMonitor(int pin);

        void loop(Timekeeper *tk);

        double getRawReading();
        double getBatteryVoltage();
        double getBatteryPercent();

        bool isBatteryDead();
        bool isStable();

    private:
        MillivoltsInputStream *_rawInput;
        SmoothedInputStream *_smoothedInput;
        ScaledInputStream *_voltsLevel;
        ScaledInputStream *_percentLevel;

        double _lastVoltageReading;
        bool _isStable;
};