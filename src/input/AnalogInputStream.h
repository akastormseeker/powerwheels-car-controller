#pragma once

#include <Arduino.h>

#include "InputStream.h"

class AnalogInputStream : public InputStream {
    public:
        AnalogInputStream(int pin);

        void loop(Timekeeper *tk);

        double getValue();

    private:
        int _pin;
};