#include <stdio.h>
#include "driver/ledc.h"
#include "main.h"
#include "pwm.h"

#define PWM_FREQUENCY 1000
#define MOTOR1_ENA_PIN 17
#define MOTOR2_ENA_PIN 16

static const char *TAG = "pwm_init";

static esp_err_t pwm_output_init()
{
    ledc_channel_config_t pwm_channel_config = {};

    pwm_channel_config.speed_mode = LEDC_LOW_SPEED_MODE;
    pwm_channel_config.channel = LEDC_CHANNEL_0;
    pwm_channel_config.timer_sel = LEDC_TIMER_0;
    pwm_channel_config.intr_type = LEDC_INTR_DISABLE;
    pwm_channel_config.gpio_num = MOTOR1_ENA_PIN;
    pwm_channel_config.duty = 0;
    pwm_channel_config.hpoint = 0;

    ERROR_CHECK(ledc_channel_config(&pwm_channel_config));

    pwm_channel_config.channel = LEDC_CHANNEL_0;
    pwm_channel_config.gpio_num = MOTOR2_ENA_PIN;

    ERROR_CHECK(ledc_channel_config(&pwm_channel_config));

    return ESP_OK;
}

static esp_err_t pwm_timer_init()
{

    ledc_timer_config_t ledc_timer = {};

    ledc_timer.speed_mode = LEDC_LOW_SPEED_MODE;
    ledc_timer.timer_num = LEDC_TIMER_0;
    ledc_timer.duty_resolution = LEDC_TIMER_13_BIT;
    ledc_timer.freq_hz = PWM_FREQUENCY;
    ledc_timer.clk_cfg = LEDC_AUTO_CLK;

    ERROR_CHECK(ledc_timer_config(&ledc_timer));

    return ESP_OK;
}

esp_err_t pwm_init()
{
    ERROR_CHECK(pwm_timer_init());

    ERROR_CHECK(pwm_output_init());

    SET_PWM_VALUE(MOTOR1_CHANNEL, 0);
    SET_PWM_VALUE(MOTOR2_CHANNEL, 0);

    INFO_LOG(TAG, "complete");

    return ESP_OK;
}
