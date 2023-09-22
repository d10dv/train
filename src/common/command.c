#include "command.h"
#include "periphery/gpio.h"
#include "periphery/pwm.h"

void start(void)
{
    PIN_SET(MOTOR_IN1_PIN);
    PIN_RESET(MOTOR_IN2_PIN);
    PIN_SET(MOTOR_IN3_PIN);
    PIN_RESET(MOTOR_IN4_PIN);

    SET_PWM_VALUE(MOTOR1_CHANNEL, 4095);
    SET_PWM_VALUE(MOTOR2_CHANNEL, 4095);
}

void stop(void)
{

    PIN_RESET(MOTOR_IN1_PIN);
    PIN_RESET(MOTOR_IN2_PIN);
    PIN_RESET(MOTOR_IN3_PIN);
    PIN_RESET(MOTOR_IN4_PIN);

    SET_PWM_VALUE(MOTOR1_CHANNEL, 0);
    SET_PWM_VALUE(MOTOR2_CHANNEL, 0);
}