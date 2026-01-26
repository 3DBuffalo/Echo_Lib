#include <Arduino.h>
#include "Motor.h"
#include "MotorControllers.h"

Motor::Motor(mcpwm_unit_t unit, mcpwm_timer_t timer, int id, int pin1, int pin2)
  : mcpwm_unit(unit), timer(timer), id(id), pin1(pin1), pin2(pin2), reversed(false), currentSpeed(0) {

  mcpwm_io_signals_t signalA, signalB;

  if (unit == MCPWM_UNIT_0) {
    if (timer == MCPWM_TIMER_0) {
      signalA = MCPWM0A;
      signalB = MCPWM0B;
    } else if (timer == MCPWM_TIMER_1) {
      signalA = MCPWM1A;
      signalB = MCPWM1B;
    } else if (timer == MCPWM_TIMER_2) {
      signalA = MCPWM2A;
      signalB = MCPWM2B;
    }
  } else if (unit == MCPWM_UNIT_1) {
    if (timer == MCPWM_TIMER_0) {
      signalA = MCPWM0A;
      signalB = MCPWM0B;
    } else if (timer == MCPWM_TIMER_1) {
      signalA = MCPWM1A;
      signalB = MCPWM1B;
    } else if (timer == MCPWM_TIMER_2) {
      signalA = MCPWM2A;
      signalB = MCPWM2B;
    }
  }

  mcpwm_gpio_init(unit, signalA, pin1);
  mcpwm_gpio_init(unit, signalB, pin2);

  mcpwm_config_t pwm_config = {
    .frequency = 1000,
    .cmpr_a = 0,
    .cmpr_b = 0,
    .duty_mode = MCPWM_DUTY_MODE_0,
    .counter_mode = MCPWM_UP_COUNTER
  };
  mcpwm_init(unit, timer, &pwm_config);

  mcpwm_start(unit,timer);

  mcpwm_set_duty_type(unit,timer,MCPWM_OPR_A,MCPWM_DUTY_MODE_0);
  mcpwm_set_duty_type(unit,timer,MCPWM_OPR_B,MCPWM_DUTY_MODE_0);
}

void Motor::setSpeed(int16_t dutyCycle) {
  dutyCycle = constrain(dutyCycle, -1000, 1000);
  currentSpeed = dutyCycle;
  applySpeed(currentSpeed);
}

void Motor::applySpeed(int16_t dutyCycle) {
  if (reversed) dutyCycle = -dutyCycle;
  float dutyPercent = abs(dutyCycle) / 10.0f;

  if (dutyCycle == 0) {
    mcpwm_set_signal_low(mcpwm_unit, timer, MCPWM_OPR_A);
    mcpwm_set_signal_low(mcpwm_unit, timer, MCPWM_OPR_B);
  } else if (dutyCycle > 0) {
    mcpwm_set_duty(mcpwm_unit, timer, MCPWM_OPR_A, dutyPercent);
    mcpwm_set_duty_type(mcpwm_unit,timer,MCPWM_OPR_A,MCPWM_DUTY_MODE_0);
    mcpwm_set_signal_low(mcpwm_unit, timer, MCPWM_OPR_B);
  } else {
    mcpwm_set_duty(mcpwm_unit, timer, MCPWM_OPR_B, dutyPercent);
    mcpwm_set_duty_type(mcpwm_unit,timer,MCPWM_OPR_B,MCPWM_DUTY_MODE_0);
    mcpwm_set_signal_low(mcpwm_unit, timer, MCPWM_OPR_A);
  }
}

void Motor::reverseDirection() {
  reversed = !reversed;
}

void Motor::disablePWM() {
  mcpwm_set_duty(mcpwm_unit, timer, MCPWM_OPR_A, 0);
  mcpwm_set_duty(mcpwm_unit, timer, MCPWM_OPR_B, 0);
  currentSpeed = 0;
}

void Motor::setCoastMode() {
  disablePWM();
}

void Motor::setBrakeMode() {
  mcpwm_set_signal_low(mcpwm_unit, timer, MCPWM_OPR_A);
  mcpwm_set_signal_low(mcpwm_unit, timer, MCPWM_OPR_B);
  currentSpeed = 0;
}

int Motor::getId() const {
  return id;
}

bool Motor::isReversed() const {
  return reversed;
}