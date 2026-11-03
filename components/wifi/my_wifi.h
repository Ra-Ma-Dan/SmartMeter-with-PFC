#ifndef MY_WIFI_H
#define MY_WIFI_H

#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
/* The examples use WiFi configuration that you can set via project configuration menu.

   If you'd rather not, just change the below entries to strings with
   the config you want - ie #define EXAMPLE_ESP_WIFI_STA_SSID "mywifissid"
*/

/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */

/* STA Configuration */
#define TARGET_WIFI_SSID            "nrtwork name"
#define TARGET_WIFI_PASS            "netwoek pass"
#define MAX_RETRY_TIMES             5


/* AP Configuration */
#define ESP_WIFI_AP_SSID            "PFC-Meter-001"
#define ESP_WIFI_AP_PASSWD          "1234567890"
#define ESP_WIFI_CHANNEL            6
#define MAX_STA_CONNECTION          4
#define ESP_AUTHMODE                WIFI_AUTH_WPA2_PSK
/* Missing Compiler Anchor Macros */
#define WPA3_SAE_PWE_BOTH           2


/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT          BIT0
#define WIFI_FAIL_BIT               BIT1

/*DHCP server option*/
#define DHCPS_OFFER_DNS             0x02



void wifi_ap_sta_init(void);


#endif

