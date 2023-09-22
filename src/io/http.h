#ifndef __HTTP_H__
#define __HTTP_H__

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>
#include <esp_http_server.h>

#include "main.h"

httpd_handle_t start_webserver(void);
void stop_webserver(httpd_handle_t server);

#endif /* __HTTP_H__ */
