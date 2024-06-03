#pragma once

#include <Arduino.h>

#include "Timekeeper.h"

#define PEDAL_CALIBRATE_UP    0
#define PEDAL_CALIBRATE_DOWN  1

class Pedal {
    public:
        Pedal(int pin);

        void setUpValue(uint32_t value);
        void setDownValue(uint32_t value);
        void setMaxUpSlope(double maxSlope);
        void setMaxDownSlope(double maxSlope);
        void setMaxSlope(double maxSlope);
        uint32_t getUpValue();
        uint32_t getDownValue();
        void setUpDeadZone(double deadZone);
        void setDownDeadZone(double deadZone);
        double getUpDeadZone();
        double getDownDeadZone();
        double getMaxUpSlope();
        double getMaxDownSlope();

        void loop(Timekeeper *timekp);
        bool calibrate(int mode);

        uint32_t getRawValue();
        double getRawPosition();
        double getSmoothedPosition();
        double getPosition();

        double getFrameSlope();
        double getSlope();

        void overrideReadPercent(double percent);
        void cancelReadOverride();
        bool isOverridingRead();

    private:
        int _pin;
        uint32_t _upValue;
        uint32_t _downValue;
        uint32_t _lastCalibrationValue;
        double _upDeadZone;
        double _downDeadZone;
        double _smoothingFactor;
        double _maxUpSlope;
        double _maxDownSlope;

        bool _isOverridingRead;
        double _rawPercentOverrideValue;

        uint32_t _rawValue;
        double _rawPercent;
        double _smoothedPercent;
        double _filteredPercent;
        
        double _frameSlope;
        double _filteredSlope;

        uint32_t readRaw();
};