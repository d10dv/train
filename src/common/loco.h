#ifndef __LOCO_H__
#define __LOCO_H__

#include "main.h"

#define POWER_STEP 1

typedef enum
{
    LOCO_IDLE = 0,
    LOCO_BREAK,
    LOCO_FORWARD,
    LOCO_REVERSE,
} loco_state_t;

typedef struct
{
    int16_t power_min;
    int16_t power_max;
    int16_t power;
    int16_t setpoint_power;
    loco_state_t state;
} loco_handler_t;

extern loco_handler_t loco;

void loco_init(void);

void loco_main_task(void *pvParameters);

void loco_set_power(loco_handler_t *loco, int16_t power);

#endif // __LOCO_H__