#ifndef GPIO_PERIPHERY_H_
#define GPIO_PERIPHERY_H_

#include "main.h"

#define PIN_SET(x) gpio_set_level(x, 1)
#define PIN_RESET(x) gpio_set_level(x, 0)

#define MOTOR_IN1_PIN 25
#define MOTOR_IN2_PIN 26
#define MOTOR_IN3_PIN 32
#define MOTOR_IN4_PIN 33

esp_err_t gpio_init(void);

#endif /* GPIO_PERIPHERY_H_ */