#include <stdio.h>
#include "esp_log.h"
#include "init.h"
#include "periphery/gpio.h"
#include "periphery/pwm.h"
#include "io/bluetooth.h"
#include "io/wifi.h"
#include "periphery/h-bridge.h"
#include "io/http.h"
#include "io/spiffs.h"
#include "loco.h"

static const char *TAG = "INIT";

esp_err_t init()
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ERROR_CHECK(ret);

    ERROR_CHECK(esp_netif_init());

    ERROR_CHECK(esp_event_loop_create_default());

    ERROR_CHECK(gpio_init());

    ERROR_CHECK(pwm_init());

    spiffs_init();

    ERROR_CHECK(bluetooth_init());

    ERROR_CHECK(wifi_init());

    start_webserver();

    h_bridge_initial();

    loco_init();

    INFO_LOG(TAG, "complete");

    return ESP_OK;
}