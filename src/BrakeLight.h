#pragma once

class BrakeLight {
    public:
        BrakeLight(int pin, bool invertedOutput, long holdTime, long fadeTime);
        void setLevels(int on, int off);
        void loop();

        void activate();
        void deactivate();

    private:
        int _pin;
        bool _invertedOutput;
        int _onLevel;
        int _offLevel;
        unsigned long _holdTime;
        unsigned long _fadeTime;
        unsigned long _lastActivation;
        int _currentLevel;

        void _setLevel(int level);
};