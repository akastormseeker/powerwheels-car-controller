#pragma once

#define FLASHLIGHT_END_HOLD   0
#define FLASHLIGHT_END_OFF    1
#define FLASHLIGHT_END_ON     2

class FlashLight {
    public:
        FlashLight(int pin);

        void loop();

        void setOn();
        void setOff();
        void setBlink(int onCount, unsigned long onTimeMs, unsigned long offTimeMs, unsigned long delayTimeMs, int endMode, int loopCount);
        void setBlink(unsigned long onTimeMs, unsigned long offTimeMs, int endMode, int loopCount);

    private:
        int _pin;
        int _currentLevel;

        unsigned long _startTime;
        bool _isRunning;

        int _blinkCount;
        unsigned long _onTime;
        unsigned long _offTime;
        unsigned long _delayTime;
        int _loopCount;
        int _endMode;

        void _setLevel(int level);
        void _setOn();
        void _setOff();
};
