#pragma once

#include "driver/mcpwm.h"
#include "soc/mcpwm_reg.h"
#include "soc/mcpwm_struct.h"

#define MOTOR_DRIVE_BRAKE    0
#define MOTOR_DRIVE_FORWARD  1
#define MOTOR_DRIVE_REVERSE  2

class Motor {
    public:
        Motor(mcpwm_unit_t mcpwmUnit, int gpioA, int gpioB, int gpioEnable, bool outputInverted);

        void setDriveInverted(bool inverted);
        bool isDriveInverted();

        void setDriveMode(int mode);
        void setPower(float power);

    private:
    mcpwm_unit_t _mcpwm_unit;
        int _pin_enable;
        int _pin_a;
        int _pin_b;
        bool _drive_inverted;
        int _drive_mode;
        bool _output_inverted;
        float _current_power;

        void _setSignalLow(mcpwm_timer_t timer, mcpwm_generator_t gen);
        void _setSignalHigh(mcpwm_timer_t timer, mcpwm_generator_t gen);

        void _setPowerOffInt();
        void _setPowerOnInt();
        void _setPowerDutyInt(float duty);
};