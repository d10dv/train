#include "loco.h"
#include "periphery/h-bridge.h"
#include "utils/conversion.h"
#include <stdlib.h>

loco_handler_t loco;

uint16_t loco_get_pwm_power(loco_handler_t *loco);

void loco_init(void)
{
    loco.power_min = LOCO_POWER_MIN;
    loco.power_max = LOCO_POWER_MAX;
}

void loco_main_task(void *pvParameters)
{

    for (;;)
    {
        if (loco.power < loco.setpoint_power)
        {
            loco.power += POWER_STEP;
        }

        if (loco.power > loco.setpoint_power)
        {
            loco.power -= POWER_STEP;
        }

        if (loco.power == 0)
        {
            h_bridge_idle(&motor1_h_bridge_handler);
            h_bridge_idle(&motor1_h_bridge_handler);
        }
        else
        {
            uint16_t h_pridge_power = loco_get_pwm_power(&loco);

            h_bridge_set_power(&motor1_h_bridge_handler, h_pridge_power);
            h_bridge_set_power(&motor2_h_bridge_handler, h_pridge_power);

            h_bridge_run(&motor1_h_bridge_handler);
            h_bridge_run(&motor2_h_bridge_handler);
        }

        vTaskDelay(5);
    }
}

void loco_set_power(loco_handler_t *loco, int16_t power)
{

    if (power >= loco->power_max)
    {
        loco->setpoint_power = loco->power_max;
        return;
    }

    if (power <= loco->power_min)
    {
        loco->setpoint_power = loco->power_min;
        return;
    }

    loco->setpoint_power = power;
}

uint16_t loco_get_pwm_power(loco_handler_t *loco)
{
    uint16_t actual_power = 0;

    if (loco->power < 0)
    {
        actual_power = abs(loco->power);
    }
    else
    {
        actual_power = loco->power;
    }

    conversion_t conversion_config = {
        .hight_in = loco->power_max,
        .low_in = 0,
        .hight_out = COMMON_SCALE_MAX,
        .low_out = COMMON_SCALE_MIN,
        .input = actual_power};

    return conversion(&conversion_config);
}
