#include "wifi.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "freertos/event_groups.h"
#include "nvs_flash.h"
#include "esp_mac.h"

static const char* TAG = "WiFi driver";

//Wifi event bits
#define WIFI_CONNECTED_BIT      BIT0
#define WIFI_FAILED_BIT         BIT1

#define WIFI_AP_STACONNECTED    BIT0
#define WIFI_AP_STASTOPPED      BIT1

//connection to a hotspot
#define TARGET_AP_SSID          "hotspot name"
#define TARGET_AP_PASS          "hotspt password"
#define MAX_RETRIES              10
int num_tries = 0;

//creating Access point
#define AP_SSID                 "ESP32_PFC_METER"
#define AP_PASS                 "pfc_meter"
#define AP_CHANNEL              "6"
#define AP_AUTHMODE             "WIFI_AUTH_WAP2"

EventGroupHandle_t wifi_event_handle;
EventGroupHandle_t ip_event_handle;

static void IRAM_ATTR wifi_event_handler_fxn(void *args, esp_event_base_t event_base, int16_t event_id, void *event_data){

    if(event_base == WIFI_EVENT){
        if(event_id == WIFI_EVENT_STA_START){
            esp_wifi_connect();
        } else if(event_id == WIFI_EVENT_STA_DISCONNECTD){
            if (num_tries < MAX_RETRIES){
                esp_wifi_connect();
                ESP_LOGI(TAG, "Retrying to connect to network: %s", TARGET_AP_SSID);
                num_tries++;
            }else{
                num_tries = 0;
                xEventGroupSetBits(wifi_event_handle, WIFI_FAILED_BIT)
            }
            ESP_LOGI(TAG, "Disconnected");
        }
    } else if(event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP){
        ip_event_got_ip_t *event = (ip_event_got_ip_t *) event_data;
        xEventGroupSetBits(wifi_event_handle, WIFI_CONNECTED_BIT)
        ESP_LOGI(TAG, "connected to ")
    }
}