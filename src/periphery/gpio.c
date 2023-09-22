#include <stdio.h>
#include <string.h>
#include "gpio.h"
#include "driver/gpio.h"

#define GPIO_OUTPUT_PIN_SEL ((1ULL << MOTOR_IN1_PIN) | (1ULL << MOTOR_IN2_PIN) | (1ULL << MOTOR_IN3_PIN) | (1ULL << MOTOR_IN4_PIN))

static const char *TAG = "gpio_init";

esp_err_t gpio_init()
{
    gpio_config_t gpio_general_conf = {};
    gpio_general_conf.intr_type = GPIO_INTR_DISABLE;
    gpio_general_conf.mode = GPIO_MODE_OUTPUT;
    gpio_general_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    gpio_general_conf.pull_down_en = 0;
    gpio_general_conf.pull_up_en = 0;

    ERROR_CHECK(gpio_config(&gpio_general_conf));

    PIN_RESET(MOTOR_IN1_PIN);
    PIN_RESET(MOTOR_IN2_PIN);
    PIN_RESET(MOTOR_IN3_PIN);
    PIN_RESET(MOTOR_IN4_PIN);

    INFO_LOG(TAG, "complete");

    return ESP_OK;
}