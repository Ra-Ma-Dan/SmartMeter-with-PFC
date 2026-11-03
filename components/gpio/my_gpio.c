#include "esp_rom_sys.h"
#include "my_gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_err.h"

static const char *TAG = "GPIO Driver";

gpio_callbacks_t wifi_cb = NULL;


void wifi_btn_task(void *pvParameters){
   while(true) {
    if(gpio_get_level(WIFI_BTN_PIN) == 0){
        vTaskDelay(pdMS_TO_TICKS(wifi_hold_ms));

        if(gpio_get_level(WIFI_BTN_PIN) == 0){
            if (wifi_cb != NULL){
                wifi_cb();
                ESP_LOGI(TAG, "WiFi On");
            }
            while (gpio_get_level(WIFI_BTN_PIN) == 0) esp_rom_delay_us(20);
            
        }
    }
    vTaskDelay(pdMS_TO_TICKS(50));
   }
}

void gpio_pins_config(){
    gpio_config_t wifi_btn = {
        .pin_bit_mask = (1ULL << WIFI_BTN_PIN) |
        (1ULL << WIFI_BTN_PIN),
        .mode         = GPIO_MODE_INPUT,
        .pull_up_en   = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type    = GPIO_INTR_DISABLE 
    };
    gpio_config(&wifi_btn);
    xTaskCreate(wifi_btn_task, "wifi_control_task", 2048, NULL, 5, NULL);

    gpio_config_t led_pins = {
        .pin_bit_mask = (1ULL << PF_LED_PIN) |
                        (1ULL << WIFI_BTN_PIN),
        .mode         = GPIO_MODE_OUTPUT,
        .pull_up_en   = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type    = GPIO_INTR_DISABLE
    };
    gpio_config(&led_pins);


    gpio_config_t relay_pins = {
        .pin_bit_mask = (1ULL << RELAY_1_PIN) |
                        (1ULL << RELAY_2_PIN) |
                        (1ULL << RELAY_3_PIN) |
                        (1ULL << RELAY_4_PIN),
        .mode         = GPIO_MODE_OUTPUT,
        .pull_up_en   = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type    = GPIO_INTR_DISABLE
    };
    gpio_config(&relay_pins);
    ESP_LOGI(TAG, "GPIOs Initialized");

}



// ########### Set Callbacks functions ##################
void gpio_driver_register_wifi_callback(gpio_callbacks_t cb_fxn){
    wifi_cb = cb_fxn;
};