#pragma once

#include <Arduino.h>

#include "InputStream.h"

class MillivoltsInputStream : public InputStream {
    public:
        MillivoltsInputStream(int pin);

        void loop(Timekeeper *tk);

        double getValue();

    private:
        int _pin;
};