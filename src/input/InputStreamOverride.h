#pragma once

#include <Arduino.h>
#include "InputStream.h"

class InputStreamOverride : public InputStream {
    public:
        InputStreamOverride(InputStream *source);

        void loop(Timekeeper *tk);
        double getValue();

        void setOverrideValue(double value);
        double getOverrideValue();
        void setOverrideWeight(double weight);
        double getOverrideWeight();
        bool isOverridingInput();

    private:
        InputStream *_source;
        double _overrideValue;
        double _weight;
};