#ifndef __MAIN_H__
#define __MAIN_H__

#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "freertos/event_groups.h"
#include "esp_netif.h"
#include "config.h"

#define ERROR_CHECK(x) ESP_ERROR_CHECK(x)

#define ERROR_LOG(...) ESP_LOGE(__VA_ARGS__)
#define WARNING_LOG(...) ESP_LOGW(__VA_ARGS__)
#define INFO_LOG(...) ESP_LOGI(__VA_ARGS__)
#define DEBUG_LOG(...) ESP_LOGD(__VA_ARGS__)
#define VERBOSE_LOG(...) ESP_LOGV(__VA_ARGS__)

#endif /*  __MAIN_H__ */