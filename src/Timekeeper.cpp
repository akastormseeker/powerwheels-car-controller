#include <Arduino.h>

#include "Timekeeper.h"

Timekeeper::Timekeeper() {
    lastFrameMicros = micros();
    frameTime = 0.0;
}

bool Timekeeper::loop(unsigned long minFrameMicros) {
    unsigned long microsNow = micros();
    unsigned long deltaMicros = (microsNow - lastFrameMicros);
    if(deltaMicros < minFrameMicros) {
        frameTime = 0;
        return false;
    }
    lastFrameMicros = microsNow;
    frameTime = (double)deltaMicros / 1000000.0;
    runTime += frameTime;
    return true;
}