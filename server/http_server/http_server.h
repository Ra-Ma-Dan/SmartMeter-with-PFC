#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include "esp_http_server.h"
#include "esp_err.h"


typedef struct {
    float voltage;
    float current;
    float power;
    float power_factor;
} live_metrics_t;

static esp_err_t root_get_handler(httpd_req_t *request)
static esp_err_t css_get_handler(httpd_req_t *request)
static esp_err_t js_get_handler(httpd_req_t *request)
static esp_err_t data_get_handler(httpd_req_t *request)
void start_webserver(void) 




#endif