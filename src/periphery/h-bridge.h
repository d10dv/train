#ifndef __H_BRIDGE_PERIPHERAL_
#define __H_BRIDGE_PERIPHERAL_

#include "main.h"

#define PWM_MIN 0
#define PWM_MAX 8192
#define POWER_MIN COMMON_SCALE_MIN
#define POWER_MAX COMMON_SCALE_MAX

typedef enum
{
    NONE = 0,
    IDLE,
    BREAK,
    RUN,
    OPERATE
} h_bridge_state_t;

typedef struct
{
    uint16_t pwm_min;
    uint16_t pwm_max;
    uint16_t power_min;
    uint16_t power_max;
    uint16_t ena1_pin;
    uint16_t ena2_pin;
    uint16_t pwm_channel;
} h_bridge_config_t;

typedef struct
{
    h_bridge_config_t *config;
    uint16_t state;
    uint16_t power;
} h_bridge_handler_t;

extern h_bridge_handler_t motor1_h_bridge_handler;
extern h_bridge_handler_t motor2_h_bridge_handler;

void h_bridge_initial(void);

void h_bridge_handlers_task(void *pvParameters);

void h_bridge_config(h_bridge_handler_t *p, h_bridge_config_t *config);

void h_bridge_handler(h_bridge_handler_t *p);

void h_bridge_break(h_bridge_handler_t *p);

void h_bridge_run(h_bridge_handler_t *p);

void h_bridge_idle(h_bridge_handler_t *p);

void h_bridge_set_power(h_bridge_handler_t *p, uint16_t power);

#endif /* __H_BRIDGE_PERIPHERAL_ */