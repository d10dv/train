#include "run.h"
#include "periphery/h-bridge.h"
#include "loco.h"

void run()
{
    xTaskCreate(h_bridge_handlers_task, "h_bridge_handlers", 4096, NULL, 5, NULL);
    xTaskCreate(loco_main_task, "loco_main_handler", 4096, NULL, 5, NULL);
}