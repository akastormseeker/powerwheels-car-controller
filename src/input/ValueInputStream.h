#pragma once

#include <Arduino.h>
#include "InputStream.h"

class ValueInputStream : public InputStream {
    public:
        ValueInputStream(double initialValue);
        ValueInputStream();

        void loop(Timekeeper *tk);
        double getValue();

        void setValue(double value);

    private:
        double _value;
};