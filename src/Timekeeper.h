#pragma once

class Timekeeper {
    public:
        Timekeeper();
        bool loop(unsigned long minFrameMicros);

        double frameTime;
        double runTime;

    private:
        unsigned long lastFrameMicros;
};