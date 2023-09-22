#ifndef __PWM_PERIPHERY_H_
#define __PWM_PERIPHERY_H_

#include "main.h"
#include "driver/ledc.h"

#define SET_PWM_VALUE(channel, value) ({ ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, channel, value)); ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, channel)); })

#define MOTOR1_CHANNEL LEDC_CHANNEL_0
#define MOTOR2_CHANNEL LEDC_CHANNEL_1


esp_err_t pwm_init(void);

#endif /*__PWM_PERIPHERY_H_ */
