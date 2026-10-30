#include "http_server.h"


static const char *TAG = "Http Server";

esp_err_t home_get_handler(httpd_req_t *request, char data[]){
    
    httpd_req_send(request, data, HTTPD_USE_STR_LEN);
    return ESP_OK;
}
esp_err_t post_handler(httpd_req_t *request, char content[100]){

    int recv = httpd_httpd_req_recv(request, content, (size_t *)sizeof(content));
    if (recv <= 0){
        int recv = httpd_httpd_req_recv(request, content, (size_t *)sizeof(content));
    }

    return ESP_OK;
}


esp_err_t put_handler(httpd_req_t *request){

}
esp_err_t delete_handler(httpd_req_t *request){

}



httpd_uri_t home_get_uri = {
    .uri = '/',
    .method = HTTPD_GET,
    .handler = get_handler,
    .user_ctx = NULL
};

httpd_uri_t put_uri = {
    .uri = '/',
    .method = HTTPD_PUT,
    .handler = put_handler,
    .user_ctx = NULL
};

httpd_uri_t delete_uri = {
    .uri = '/',
    .method = HTTPD_DELETE,
    .handler = delete_handler,
    .user_ctx = NULL
};

httpd_uri_t post_uri {
    .uri = '/',
    .method = HTTPD_POST,
    .handler = post_handler,
    .user_ctx = NULL
};


httpd_hanlde_t http_init_start_webserver(){
    httpd_config_t server_cfg = HTTPD_CONFIG_DEFAULT;
    httpd_handle_t server_handle = NULL;
    
    esp_err_t res = httpd_start(server_handle, &server_cfg);
    if(res == ESP_OK){
        httpd_register_uri_handler(server_handle, &home_get_uri);
        httpd_register_uri_handler(server_handle, &post_uri);
        httpd_register_uri_handler(server_handle, &put_uri);
        httpd_register_uri_handler(server_handle, &delete_uri);

    }

    //server will be NULL if it fail to start
    return server;
}


esp_err_t stop_web_server(httpd_handle_t server){

    if(server != NULL){
        httpd_stop();
    }

    return ESP_OK;
}