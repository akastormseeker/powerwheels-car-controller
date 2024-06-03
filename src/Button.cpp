#include <Arduino.h>
#include "Button.h"

Button::Button(int pin, int inputMode, int activeState) {
    this->_pin = pin;
    this->_activeState = activeState;

    this->_debounceDelay = 0;

    pinMode(this->_pin, inputMode);
    this->_currentState = digitalRead(this->_pin);

    this->_onPressedCallback = NULL;
    this->_onLongPressedCallback = NULL;
    this->_onReleasedCallback = NULL;
}

void Button::setDebounceDelay(int delay) {
    this->_debounceDelay = delay;
}

void Button::setOnPressedCallback(void (*callback)()) {
    this->_onPressedCallback = callback;
}

void Button::setOnReleasedCallback(void (*callback)()) {
    this->_onReleasedCallback = callback;
}

void Button::setOnLongPressedCallback(void (*callback)(), long delay) {
    this->_onLongPressedCallback = callback;
    this->_longPressDelay = delay;
}

void Button::loop() {
    if(this->_debouncing && (millis() - this->_lastStateChange) >= this->_debounceDelay) {
        this->_debouncing = false;
    }
    if(!this->_debouncing) {
        int val = digitalRead(this->_pin);
        if(val != this->_currentState) {
            this->_currentState = val;
            this->_lastStateChange = millis();
            if(this->_currentState == this->_activeState) {
                this->_longPressHandled = false;
                if(this->_onPressedCallback != NULL) {
                    this->_onPressedCallback();
                }
            }
            else {
                if(this->_onReleasedCallback != NULL) {
                    this->_onReleasedCallback();
                }
            }
        }
    }
    if(this->_onLongPressedCallback != NULL && this->_currentState == this->_activeState && !this->_longPressHandled && ((millis() - this->_lastStateChange) >= this->_longPressDelay)) {
        this->_longPressHandled = true;
        this->_onLongPressedCallback();
    }
}

bool Button::isPressed() {
    return (this->_currentState == this->_activeState);
}

void Button::cancelLongPress() {
    this->_longPressHandled = true;
}