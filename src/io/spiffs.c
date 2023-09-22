#include "spiffs.h"
#include "esp_spiffs.h"
#include "spiffs_config.h"

static const char *TAG = "spiffs_init";

void spiffs_init()
{

    esp_vfs_spiffs_conf_t conf = {
        .base_path = "/spiffs",
        .partition_label = NULL,
        .max_files = 5,
        .format_if_mount_failed = true};

    esp_err_t ret = esp_vfs_spiffs_register(&conf);

    if (ret != ESP_OK)
    {
        if (ret == ESP_FAIL)
        {
            ERROR_LOG(TAG, "Failed to mount or format filesystem");
        }
        else if (ret == ESP_ERR_NOT_FOUND)
        {
            ERROR_LOG(TAG, "Failed to find SPIFFS partition");
        }
        else
        {
            ERROR_LOG(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return;
    }

    size_t total = 0, used = 0;
    ret = esp_spiffs_info(conf.partition_label, &total, &used);
    if (ret != ESP_OK)
    {
        ERROR_LOG(TAG, "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
    }
    else
    {
        INFO_LOG(TAG, "Partition size: total: %d, used: %d", total, used);
    }
}