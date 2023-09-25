#include "command.h"
#include "periphery/gpio.h"
#include "periphery/pwm.h"
#include "loco.h"

void start(void)
{
    loco_set_power(&loco, 50);
}

void stop(void)
{

    loco_set_power(&loco, 0);
}