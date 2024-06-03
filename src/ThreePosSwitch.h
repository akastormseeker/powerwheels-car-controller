#pragma once

#define SWITCH_POS_A    1
#define SWITCH_POS_OFF  0
#define SWITCH_POS_B    -1

class ThreePosSwitch {
    public:
        ThreePosSwitch(int pin1, int pin2, int inputMode, int activeState);
        void setOnChangedCallback(void (*callback)(ThreePosSwitch* sender, int newState));
        void setDebounceDelay(unsigned int delay);
        void loop();

        int getPosition();
        bool isOff();
        bool isPosA();
        bool isPosB();

    private:
        int _pin1;
        int _pin2;
        int _activeState;
        unsigned int _debounceDelay;

        int _lastPin1Val;
        unsigned long _pin1Pause;
        int _lastPin2Val;
        unsigned long _pin2Pause;

        int _pos;

        void (*_onChangedCallback)(ThreePosSwitch* sender, int newState);
};