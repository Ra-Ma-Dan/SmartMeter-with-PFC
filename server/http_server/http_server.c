#include "http_server.h"
#include "esp_http_server.h"
#include "esp_log.h"
#include "sdkconfig.h"

static const char *TAG = "HTTP_SERVER";

// 1. Embed the binary text files out of flash memory strings
extern const uint8_t index_html_start[] asm("_binary_index_html_start");
extern const uint8_t index_html_end[]   asm("_binary_index_html_end");
extern const uint8_t style_css_start[]  asm("_binary_style_css_start");
extern const uint8_t style_css_end[]    asm("_binary_style_css_end");
extern const uint8_t scripts_js_start[] asm("_binary_scripts_js_start");
extern const uint8_t scripts_js_end[]   asm("_binary_scripts_js_end");

// Global telemetry structure mock (Link this to your actual PZEM global variables)

live_metrics_t live_pfc_data = {0}; // Sample base values

// Handler for HTML Webpage (root /)
static esp_err_t root_get_handler(httpd_req_t *request) {
    size_t len = index_html_end - index_html_start;
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, (const char *)index_html_start, len);
    return ESP_OK;
}

// Handler for CSS Stylesheet
static esp_err_t css_get_handler(httpd_req_t *request) {
    size_t len = style_css_end - style_css_start;
    httpd_resp_set_type(req, "text/css");
    httpd_resp_send(req, (const char *)style_css_start, len);
    return ESP_OK;
}

// Handler for Javascript engine
static esp_err_t js_get_handler(httpd_req_t *request) {
    size_t len = scripts_js_end - scripts_js_start;
    httpd_resp_set_type(req, "application/javascript");
    httpd_resp_send(req, (const char *)scripts_js_start, len);
    return ESP_OK;
}

// 2. The AJAX /data API handler returning structured JSON
static esp_err_t data_get_handler(httpd_req_t *request) {
    char json_string[128];
    
    // Convert current float parameters into an array format for JavaScript parsing
    snprintf(json_string, sizeof(json_string),
             "{\"voltage\":%.1f,\"current\":%.2f,\"power\":%.1f,\"pf\":%.2f}",
             live_pfc_data.voltage, live_pfc_data.current, 
             live_pfc_data.power, live_pfc_data.power_factor);

    httpd_resp_set_type(request, "application/json");
    // Enable CORS to make cross-origin debugging via Wokwi smooth
    httpd_resp_set_hdr(request, "Access-Control-Allow-Origin", "*"); 
    httpd_resp_send(request, json_string, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

// Register URI Paths
static const httpd_uri_t root = { 
    .uri = "/", 
    .method = HTTP_GET, 
    .handler = root_get_handler 
};
static const httpd_uri_t css  = { 
    .uri = "/style.css", 
    .method = HTTP_GET, 
    .handler = css_get_handler 
};
static const httpd_uri_t js   = { 
    .uri = "/scripts.js", 
    .method = HTTP_GET, 
    .handler = js_get_handler 
};
static const httpd_uri_t data = { 
    .uri = "/data", 
    .method = HTTP_GET, 
    .handler = data_get_handler 
};

void start_webserver(void) {
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.server_port = CONFIG_PFC_METER_SERVER_PORT; // Pulls from your Kconfig file!

    ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_register_uri_handler(server, &root);
        httpd_register_uri_handler(server, &css);
        httpd_register_uri_handler(server, &js);
        httpd_register_uri_handler(server, &data);
    }
}
