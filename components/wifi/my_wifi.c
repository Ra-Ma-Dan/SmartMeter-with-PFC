#include <stdio.h>
#include <string.h>
#include <stdarg.h>  

#include "esp_err.h"
#include "esp_log.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_event.h"

#include "esp_netif.h"
#include "esp_wifi.h"
#include "esp_netif_net_stack.h"
#include "esp_mac.h"
#include "lwip/inet.h"
#include "lwip/netdb.h"
#include "lwip/sockets.h"
#include "lwip/arch.h"
#include "lwip/lwip_napt.h"

#include "my_nvs.h"
#include "my_wifi.h"


static const char *TAG_AP = "WiFi SoftAP";
static const char *TAG_STA = "WiFi Sta";

int s_retry_num = 0;

static EventGroupHandle_t s_wifi_event_group;

static void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data){
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_AP_STACONNECTED) {
        wifi_event_ap_staconnected_t *event = (wifi_event_ap_staconnected_t *) event_data;
        ESP_LOGI(TAG_AP, "Station "MACSTR" joined, AID=%d",
                 MAC2STR(event->mac), event->aid);
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_AP_STADISCONNECTED) {
        wifi_event_ap_stadisconnected_t *event = (wifi_event_ap_stadisconnected_t *) event_data;
        ESP_LOGI(TAG_AP, "Station "MACSTR" left, AID=%d, reason:%d",
                 MAC2STR(event->mac), event->aid, event->reason);
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
        ESP_LOGI(TAG_STA, "Station started");
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        if (s_retry_num < MAX_RETRY_TIMES) {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(TAG_STA, "Retry connecting to the AP (%d of %d)", s_retry_num, MAX_RETRY_TIMES);
        } else {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT); 
        }
        ESP_LOGI(TAG_STA,"Connect to the AP failed");

    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *) event_data;
        ESP_LOGI(TAG_STA, "Got IP:" IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_ASSIGNED_IP_TO_CLIENT) {
        const ip_event_assigned_ip_to_client_t *client = (const ip_event_assigned_ip_to_client_t *)event_data;
        ESP_LOGI(TAG_AP, "Assigned IP to client: " IPSTR ", MAC=" MACSTR ", hostname='%s'",
                 IP2STR(&client->ip), MAC2STR(client->mac), client->hostname);
    } 

}

// Initialize WiFi AP mode
esp_netif_t *wifi_init_softap(void){
    esp_netif_t *esp_netif_ap = esp_netif_create_default_wifi_ap();

     // 1. Zero initialize the configuration structure
    wifi_config_t wifi_ap_config = {0}; 
    
    // 2. Safely copy strings onto the arrays
    strlcpy((char*)wifi_ap_config.ap.ssid, ESP_WIFI_AP_SSID, sizeof(wifi_ap_config.ap.ssid));
    strlcpy((char*)wifi_ap_config.ap.password, ESP_WIFI_AP_PASSWD, sizeof(wifi_ap_config.ap.password));
    
    // 3. Assign integers directly
    wifi_ap_config.ap.ssid_len = strlen(ESP_WIFI_AP_SSID);
    wifi_ap_config.ap.channel = ESP_WIFI_CHANNEL;
    wifi_ap_config.ap.max_connection = MAX_STA_CONNECTION;
    wifi_ap_config.ap.authmode = ESP_AUTHMODE;
    wifi_ap_config.ap.pmf_cfg.required = false;

    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_ap_config));

    ESP_LOGI(TAG_AP, "wifi_init_softap finished. SSID:%s password:%s channel:%d",
             ESP_WIFI_AP_SSID, ESP_WIFI_AP_PASSWD, ESP_WIFI_CHANNEL);

    return esp_netif_ap;
}

//Initializing STA mode
esp_netif_t *wifi_init_sta(void){
    esp_netif_t *esp_netif_sta = esp_netif_create_default_wifi_sta();

    wifi_config_t wifi_sta_config = {
        .sta = {
            .scan_method = 0,
            .failure_retry_cnt = MAX_RETRY_TIMES,
            .threshold.authmode = ESP_AUTHMODE,
            .sae_pwe_h2e = WPA3_SAE_PWE_BOTH,
        },
    };
    strlcpy((char*)wifi_sta_config.sta.ssid, TARGET_WIFI_SSID, sizeof(wifi_sta_config.sta.ssid));
    strlcpy((char*)wifi_sta_config.sta.password, TARGET_WIFI_PASS, sizeof(wifi_sta_config.sta.password));

    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_sta_config) );

    ESP_LOGI(TAG_STA, "wifi_init_sta finished.");

    return esp_netif_sta;
}

void softap_set_dns_addr(esp_netif_t *esp_netif_ap, esp_netif_t *esp_netif_sta){
    esp_netif_dns_info_t dns;
    esp_netif_get_dns_info(esp_netif_sta, ESP_NETIF_DNS_MAIN, &dns);
    uint8_t dhcps_offer_option = DHCPS_OFFER_DNS;
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_netif_dhcps_stop(esp_netif_ap));
    ESP_ERROR_CHECK(esp_netif_dhcps_option(esp_netif_ap, ESP_NETIF_OP_SET, ESP_NETIF_DOMAIN_NAME_SERVER, &dhcps_offer_option, sizeof(dhcps_offer_option)));
    ESP_ERROR_CHECK(esp_netif_set_dns_info(esp_netif_ap, ESP_NETIF_DNS_MAIN, &dns));
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_netif_dhcps_start(esp_netif_ap));
}

void wifi_ap_sta_init() {

    nvs_init(); 

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());


    /* Initialize event group */
    s_wifi_event_group = xEventGroupCreate();

    /* Register Event handler */
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                    ESP_EVENT_ANY_ID,
                    &wifi_event_handler,
                    NULL,
                    NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                    IP_EVENT_STA_GOT_IP,
                    &wifi_event_handler,
                    NULL,
                    NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                    IP_EVENT_ASSIGNED_IP_TO_CLIENT,
                    &wifi_event_handler,
                    NULL,
                    NULL));

    /*Initialize WiFi */
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));

    /* Initialize AP */
    ESP_LOGI(TAG_AP, "ESP_WIFI_MODE_AP");
    esp_netif_t *esp_netif_ap = wifi_init_softap();

    /* Initialize STA */
    ESP_LOGI(TAG_STA, "ESP_WIFI_MODE_STA");
    esp_netif_t *esp_netif_sta = wifi_init_sta();

    /* Start WiFi */
    ESP_ERROR_CHECK(esp_wifi_start());

    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
                                           WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
                                           pdFALSE,
                                           pdFALSE,
                                           pdMS_TO_TICKS(10000));

    if (bits & WIFI_CONNECTED_BIT) {
        ESP_LOGI(TAG_STA, "connected to ap SSID:%s password:%s",
                 TARGET_WIFI_SSID, TARGET_WIFI_PASS);
        softap_set_dns_addr(esp_netif_ap,esp_netif_sta);
    } else if (bits & WIFI_FAIL_BIT) {
        ESP_LOGI(TAG_STA, "Failed to connect to SSID:%s, password:%s",
                 TARGET_WIFI_SSID, TARGET_WIFI_PASS);
        ESP_LOGW(TAG_STA, "Router network not found. Running web server locally via hotspot.");
    } else {
        ESP_LOGE(TAG_STA, "UNEXPECTED EVENT");
        return;
    }

    /* Set sta as the default interface */
    esp_netif_set_default_netif(esp_netif_sta);

    /* Enable napt on the AP netif */
    if (esp_netif_napt_enable(esp_netif_ap) != ESP_OK) {
        ESP_LOGE(TAG_STA, "NAPT not enabled on the netif: %p", esp_netif_ap);
    }
}
