#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_err.h"
#include "nvs_flash.h"

#include "my_gpio.h"
#include "my_wifi.h"
#include "pmez.h"
#include "http_server.h" // <-- Include your server functions

// Inform the linker to look for the shared data struct declared in http_server.c
extern pzem_data_t live_pfc_data; 

void app_main() {
    // 1. Always initialize NVS flash storage first (Required for Wi-Fi)

    // 2. Initialize drivers correctly (No type declarations inside calls)
    wifi_ap_sta_init();
    pmez_init();
    start_webserver(); 

    printf("PFC Meter Setup Completed Successfully! Starting monitoring loop...\n");

    while (true) {
        // Pass the address of our shared structure directly to the reading driver
        if (pmez_read_data(&live_pfc_data)) {
            printf("Read successful: V=%.1fV, P=%.1fW\n", live_pfc_data.voltage, live_pfc_data.power);
        } else {
            printf("Error reading parameters from PZEM module.\n");
        }

        // Delay task loop to prevent watchdog timeouts
        vTaskDelay(pdMS_TO_TICKS(1000)); // Reading every 1 second is standard for PZEM
    }
}
