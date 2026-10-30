#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include "esp_https_server.h"
#include "esp_err.h"




esp_err_t http_init_start();

esp_err_t get_handler(httpd_req_t *request, char data[]);

esp_err_t post_handler(httpd_req_t *request, char content[100]);

esp_err_t put_handler(httpd_req_t *request)

esp_err_t delete_handler(httpd_req_t *request);

esp_err_t stop_web_server(httpd_handle_t server);






#endif