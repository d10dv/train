#include "h-bridge.h"
#include "periphery/gpio.h"
#include "periphery/pwm.h"
#include "utils/conversion.h"

static const char *TAG = "h-bridge";

h_bridge_handler_t motor1_h_bridge_handler;
h_bridge_handler_t motor2_h_bridge_handler;
h_bridge_config_t config_bridge_handler;

static h_bridge_config_t motor1_config =
    {
        .pwm_min = PWM_MIN,
        .pwm_max = PWM_MAX,
        .power_min = POWER_MIN,
        .power_max = POWER_MAX,
        .ena1_pin = MOTOR_IN1_PIN,
        .ena2_pin = MOTOR_IN2_PIN,
        .pwm_channel = MOTOR1_CHANNEL,
};

static h_bridge_config_t motor2_config =
    {
        .pwm_min = PWM_MIN,
        .pwm_max = PWM_MAX,
        .power_min = POWER_MIN,
        .power_max = POWER_MAX,
        .ena1_pin = MOTOR_IN3_PIN,
        .ena2_pin = MOTOR_IN4_PIN,
        .pwm_channel = MOTOR2_CHANNEL,
};

void h_bridge_initial()
{

    h_bridge_config(&motor1_h_bridge_handler, &motor1_config);
    h_bridge_config(&motor2_h_bridge_handler, &motor2_config);

    INFO_LOG(TAG, "init complete");
}

static void h_bridge_handle_run(h_bridge_handler_t *p);
static void h_bridge_handle_break(h_bridge_handler_t *p);
static void h_bridge_handle_idle(h_bridge_handler_t *p);

void h_bridge_handlers_task(void *pvParameters)
{
    INFO_LOG(TAG, "h-bridge handlers task RUN");

    for (;;)
    {
        h_bridge_handler(&motor1_h_bridge_handler);
        h_bridge_handler(&motor2_h_bridge_handler);

        vTaskDelay(10);
    }
}

void h_bridge_config(h_bridge_handler_t *p, h_bridge_config_t *config)
{
    p->config = config;
}

void h_bridge_handler(h_bridge_handler_t *p)
{

    switch (p->state)
    {
    case IDLE:
        h_bridge_handle_idle(p);
        break;

    case BREAK:
        h_bridge_handle_break(p);
        break;

    case RUN:
        h_bridge_handle_run(p);
        break;

    default:
        h_bridge_handle_idle(p);
        break;
    }
}

void h_bridge_idle(h_bridge_handler_t *p)
{
    INFO_LOG(TAG, "h-bridge state: %d", IDLE);

    p->state = IDLE;
}

void h_bridge_break(h_bridge_handler_t *p)
{
    INFO_LOG(TAG, "h-bridge state: %d", BREAK);

    p->state = BREAK;
}

void h_bridge_run(h_bridge_handler_t *p)
{
    INFO_LOG(TAG, "h-bridge state: %d", RUN);

    p->state = RUN;
}

void h_bridge_set_power(h_bridge_handler_t *p, uint16_t power)
{
    if (power >= p->config->power_max)
    {
        p->power = p->config->power_max;
        return;
    }

    if (power < p->config->power_min)
    {
        p->power = p->config->power_min;
        return;
    }

    p->power = power;
}

static void h_bridge_handle_run(h_bridge_handler_t *p)

{

    conversion_t conversion_config = {
        .hight_in = p->config->power_max,
        .low_in = p->config->power_min,
        .hight_out = p->config->pwm_max,
        .low_out = p->config->pwm_min,
        .input = p->power};

    uint16_t pwm_output = conversion(&conversion_config);

    PIN_SET(p->config->ena1_pin);
    PIN_RESET(p->config->ena2_pin);

    SET_PWM_VALUE(p->config->pwm_channel, pwm_output);
}

static void h_bridge_handle_break(h_bridge_handler_t *p)
{
    PIN_SET(p->config->ena1_pin);
    PIN_SET(p->config->ena2_pin);

    SET_PWM_VALUE(p->config->pwm_channel, p->config->pwm_max);
}

static void h_bridge_handle_idle(h_bridge_handler_t *p)
{
    PIN_RESET(p->config->ena1_pin);
    PIN_RESET(p->config->ena2_pin);

    SET_PWM_VALUE(p->config->pwm_channel, 0);
}
