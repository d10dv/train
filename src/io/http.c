#include "http.h"
#include <sys/param.h>
#include "esp_vfs.h"
#include "common/command.h"

#define IS_FILE_EXT(filename, ext) \
    (strcasecmp(&filename[strlen(filename) - sizeof(ext) + 1], ext) == 0)

#define FILE_PATH_MAX (ESP_VFS_PATH_MAX + CONFIG_SPIFFS_OBJ_NAME_LEN)
#define SCRATCH_BUFSIZE 8192
struct file_server_data
{
    /* Base path of file storage */
    char base_path[ESP_VFS_PATH_MAX + 1];

    /* Scratch buffer for temporary storage during file transfer */
    char scratch[SCRATCH_BUFSIZE];
};

static const char *TAG = "http_server";

static esp_err_t set_content_type_from_file(httpd_req_t *req, const char *filename)
{
    if (IS_FILE_EXT(filename, ".pdf"))
    {
        return httpd_resp_set_type(req, "application/pdf");
    }
    else if (IS_FILE_EXT(filename, ".html"))
    {
        return httpd_resp_set_type(req, "text/html");
    }
    else if (IS_FILE_EXT(filename, ".jpeg"))
    {
        return httpd_resp_set_type(req, "image/jpeg");
    }
    else if (IS_FILE_EXT(filename, ".ico"))
    {
        return httpd_resp_set_type(req, "image/x-icon");
    }
    else if (IS_FILE_EXT(filename, ".css"))
    {
        return httpd_resp_set_type(req, "text/css");
    }
    return httpd_resp_set_type(req, "text/plain");
}
//-------------------------------------------------------------
static const char *get_path_from_uri(char *dest, const char *base_path, const char *uri, size_t destsize)
{
    const size_t base_pathlen = strlen(base_path);
    size_t pathlen = strlen(uri);
    const char *quest = strchr(uri, '?');
    if (quest)
    {
        pathlen = MIN(pathlen, quest - uri);
    }
    const char *hash = strchr(uri, '#');
    if (hash)
    {
        pathlen = MIN(pathlen, hash - uri);
    }
    if (base_pathlen + pathlen + 1 > destsize)
    {
        return NULL;
    }
    strcpy(dest, base_path);
    strlcpy(dest + base_pathlen, uri, pathlen + 1);
    return dest + base_pathlen;
}
//-------------------------------------------------------------
static esp_err_t get_handler(httpd_req_t *req)
{
    char filepath[FILE_PATH_MAX];
    FILE *fd = NULL;
    char *query_params_buf;
    size_t query_buf_len;
    struct stat file_stat;

    const char *filename = get_path_from_uri(filepath, ((struct file_server_data *)req->user_ctx)->base_path,
                                             req->uri, sizeof(filepath));
    if (!filename)
    {
        ERROR_LOG(TAG, "Filename is too long");
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Filename too long");
        return ESP_FAIL;
    }
    if (strcmp(filename, "/") == 0)
    {
        strcat(filepath, "index.html");
    }
    stat(filepath, &file_stat);
    fd = fopen(filepath, "r");
    if (!fd)
    {
        ERROR_LOG(TAG, "Failed to read existing file : %s", filepath);
        /* Respond with 500 Internal Server Error */
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to read existing file");
        return ESP_FAIL;
    }
    INFO_LOG(TAG, "Sending file : %s (%ld bytes)...", filename, file_stat.st_size);
    set_content_type_from_file(req, filename);

    query_buf_len = httpd_req_get_url_query_len(req) + 1;
    INFO_LOG(TAG, "query params len: %d\n", query_buf_len);

    if (query_buf_len > 1)
    {
        query_params_buf = malloc(query_buf_len);

        if (httpd_req_get_url_query_str(req, query_params_buf, query_buf_len) == ESP_OK)
        {
            INFO_LOG(TAG, "GET query params: %s\n", query_params_buf);
            char param[32];
            if (httpd_query_key_value(query_params_buf, "command", param, sizeof(param)) == ESP_OK)
            {
                ESP_LOGI(TAG, "Found URL query parameter => command:%s", param);
                if (!strcmp(param, "start"))
                {
                    ESP_LOGI(TAG, "start!!!!!!!!!!!!!!!!");
                    start();
                }

                else if (!strcmp(param, "stop"))
                {
                    ESP_LOGI(TAG, "stop!!!!!!!!!!!!!!!!");
                    stop();
                }
            }
            else
            {
                ESP_LOGI(TAG, "Unknown command");
            }
        }
        free(query_params_buf);
    }

    char *chunk = ((struct file_server_data *)req->user_ctx)->scratch;
    size_t chunksize;
    do
    {
        chunksize = fread(chunk, 1, SCRATCH_BUFSIZE, fd);
        if (chunksize > 0)
        {
            if (httpd_resp_send_chunk(req, chunk, chunksize) != ESP_OK)
            {
                fclose(fd);
                ERROR_LOG(TAG, "File sending failed!");
                httpd_resp_sendstr_chunk(req, NULL);
                httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to send file");
                return ESP_FAIL;
            }
        }
    } while (chunksize != 0);
    fclose(fd);
    INFO_LOG(TAG, "File sending complete");
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}

/* Our URI handler function to be called during POST /uri request */
esp_err_t post_handler(httpd_req_t *req)
{
    /* Destination buffer for content of HTTP POST request.
     * httpd_req_recv() accepts char* only, but content could
     * as well be any binary data (needs type casting).
     * In case of string data, null termination will be absent, and
     * content length would give length of string */
    char content[100];

    /* Truncate if content length larger than the buffer */
    size_t recv_size = MIN(req->content_len, sizeof(content));

    int ret = httpd_req_recv(req, content, recv_size);
    if (ret <= 0)
    { /* 0 return value indicates connection closed */
        /* Check if timeout occurred */
        if (ret == HTTPD_SOCK_ERR_TIMEOUT)
        {
            /* In case of timeout one can choose to retry calling
             * httpd_req_recv(), but to keep it simple, here we
             * respond with an HTTP 408 (Request Timeout) error */
            httpd_resp_send_408(req);
        }
        /* In case of error, returning ESP_FAIL will
         * ensure that the underlying socket is closed */
        return ESP_FAIL;
    }

    INFO_LOG(TAG, "POST query params: %s\n", content);

    /* Send a simple response */
    const char resp[] = "POST ok";
    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

//-------------------------------------------------------------
httpd_handle_t start_webserver(void)
{
    httpd_handle_t server = NULL;

    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    static struct file_server_data *server_data = NULL;
    if (server_data)
    {
        ERROR_LOG(TAG, "File server already started");
        return NULL;
    }
    server_data = calloc(1, sizeof(struct file_server_data));
    if (!server_data)
    {
        ERROR_LOG(TAG, "Failed to allocate memory for server data");
        return NULL;
    }
    strlcpy(server_data->base_path, "/spiffs",
            sizeof(server_data->base_path));
    config.uri_match_fn = httpd_uri_match_wildcard;

    config.lru_purge_enable = true;

    INFO_LOG(TAG, "Starting server on port: '%d'", config.server_port);

    INFO_LOG(TAG, "Starting server on port: '%d'", config.server_port);

    if (httpd_start(&server, &config) != ESP_OK)
    {
        INFO_LOG(TAG, "Error starting server!");
        return NULL;
    }

    INFO_LOG(TAG, "Registering URI handlers");

    httpd_uri_t get_config = {
        .uri = "/*", // Match all URIs of type /path/to/file
        .method = HTTP_GET,
        .handler = get_handler,
        .user_ctx = server_data // Pass server data as context
    };

    httpd_uri_t post_config = {
        .uri = "/*",
        .method = HTTP_POST,
        .handler = post_handler,
        .user_ctx = NULL};

    httpd_register_uri_handler(server, &get_config);
    httpd_register_uri_handler(server, &post_config);

    return server;
}
//-------------------------------------------------------------
void stop_webserver(httpd_handle_t server)
{
    httpd_stop(server);
}
//-------------------------------------------------------------
