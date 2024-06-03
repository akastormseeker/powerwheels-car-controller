#pragma once

class Button {
    public:
        Button(int pin, int inputMode, int activeState);
        void setDebounceDelay(int delay);
        void setOnPressedCallback(void (*callback)());
        void setOnReleasedCallback(void (*callback)());
        void setOnLongPressedCallback(void (*callback)(), long delay);

        void loop();
        bool isPressed();
        void cancelLongPress();

    private:
        int _pin;
        int _activeState;
        int _debounceDelay;

        int _currentState;
        long _lastStateChange;
        bool _debouncing;

        void (*_onPressedCallback)();
        void (*_onReleasedCallback)();
        void (*_onLongPressedCallback)();
        long _longPressDelay;
        bool _longPressHandled;
};