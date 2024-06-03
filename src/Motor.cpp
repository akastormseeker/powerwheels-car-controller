#include <Arduino.h>
#include "Motor.h"

#include "driver/mcpwm.h"
#include "soc/mcpwm_reg.h"
#include "soc/mcpwm_struct.h"

Motor::Motor(mcpwm_unit_t mcpwmUnit, int gpioA, int gpioB, int gpioEnable, bool outputInverted) {
    this->_mcpwm_unit = mcpwmUnit;
    this->_pin_enable = gpioEnable;
    this->_pin_a = gpioA;
    this->_pin_b = gpioB;
    this->_output_inverted = outputInverted;

    mcpwm_gpio_init(this->_mcpwm_unit, MCPWM0A, this->_pin_a);
    mcpwm_gpio_init(this->_mcpwm_unit, MCPWM0B, this->_pin_b);
    mcpwm_gpio_init(this->_mcpwm_unit, MCPWM1A, this->_pin_enable);

    mcpwm_config_t pwm_config;
    pwm_config.frequency = 20000;    //frequency,
    pwm_config.cmpr_a = 0;    	 	//duty cycle of PWMxA = 0
    pwm_config.cmpr_b = 0;    	 	//duty cycle of PWMxb = 0
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;

    mcpwm_init(this->_mcpwm_unit, MCPWM_TIMER_0, &pwm_config);
    mcpwm_init(this->_mcpwm_unit, MCPWM_TIMER_1, &pwm_config);

    this->setDriveInverted(false);
    this->setDriveMode(MOTOR_DRIVE_BRAKE);
    this->setPower(0.0f);
}

void Motor::setDriveInverted(bool inverted) {
    this->_drive_inverted = inverted;
}
bool Motor::isDriveInverted() {
    return this->_drive_inverted;
}

void Motor::setDriveMode(int mode) {
    if(mode < 0 || mode > 2) return;

    this->_drive_mode = mode;

    mcpwm_generator_t gen_a = this->_drive_inverted ? MCPWM_GEN_B : MCPWM_GEN_A;
    mcpwm_generator_t gen_b = this->_drive_inverted ? MCPWM_GEN_A : MCPWM_GEN_B;

    if(this->_drive_mode == 0) { // brake
        this->_setSignalLow(MCPWM_TIMER_0, gen_a);
        this->_setSignalLow(MCPWM_TIMER_0, gen_b);
    }
    else if(this->_drive_mode == 1) { // forward
        this->_setSignalHigh(MCPWM_TIMER_0, gen_a);
        this->_setSignalLow(MCPWM_TIMER_0, gen_b);
    }
    else if(this->_drive_mode == 2) { // reverse
        this->_setSignalLow(MCPWM_TIMER_0, gen_a);
        this->_setSignalHigh(MCPWM_TIMER_0, gen_b);
    }
}

void Motor::_setPowerOffInt() {
    this->_setSignalLow(MCPWM_TIMER_1, MCPWM_GEN_A);
}

void Motor::_setPowerOnInt() {
    this->_setSignalHigh(MCPWM_TIMER_1, MCPWM_GEN_A);
}

void Motor::_setPowerDutyInt(float duty) {
    if(duty < 0.0f) duty = 0.0f;
    if(duty > 100.0f) duty = 100.0f;
    if(this->_output_inverted) {
        duty = 100.0f - duty;
    }
    mcpwm_set_duty(this->_mcpwm_unit, MCPWM_TIMER_1, MCPWM_GEN_A, duty);
    mcpwm_set_duty_type(this->_mcpwm_unit, MCPWM_TIMER_1, MCPWM_GEN_A, MCPWM_DUTY_MODE_0); //call this each time, if operator was previously in low/high state
}

void Motor::setPower(float power) {
    if(power < 0.5f) {
        power = 0.0f;
        this->_setPowerOffInt();
        return;
    }
    else if(power >= 99.5f) {
        power = 100.0f;
        this->_setPowerOnInt();
        return;
    }
    else {
        //this->_setPowerOffInt();
        this->_setPowerDutyInt(power);
    }
}

void Motor::_setSignalLow(mcpwm_timer_t timer, mcpwm_generator_t gen) {
    if(this->_output_inverted) {
        mcpwm_set_signal_high(this->_mcpwm_unit, timer, gen);
    }
    else {
        mcpwm_set_signal_low(this->_mcpwm_unit, timer, gen);
    }
}

void Motor::_setSignalHigh(mcpwm_timer_t timer, mcpwm_generator_t gen) {
    if(this->_output_inverted) {
        mcpwm_set_signal_low(this->_mcpwm_unit, timer, gen);
    }
    else {
        mcpwm_set_signal_high(this->_mcpwm_unit, timer, gen);
    }
}