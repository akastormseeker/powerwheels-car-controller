#pragma once

#include <Arduino.h>

#include "../Timekeeper.h"

class InputStream {
    public:
        virtual void loop(Timekeeper *tk) = 0;
        virtual double getValue() = 0;
};