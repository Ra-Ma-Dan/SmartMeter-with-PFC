#include "esp_rom_sys.h"
#include "gpio.h"

gpio_callbacks_t wifi_cb = NULL;
gpio_callbacks_t pf_cb = NULL;

static void IRAM_ATTR pf_btn_isr(void *pvParameters){

}

void wifi_btn_task(void *pvParameters){
   while(true) {
    if(gpio_get_level(WIFI_BTN_PIN) == 0){
        vTaskDelay(pdMS_TO_TICKS(wifi_hold_ms));

        if(gpio_get_level(WIFI_BTN_PIN) == 0){
            if (wifi_cb){
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
        .pin_bit_mask = (1ULL << PF_BTN_PIN) |
        (1ULL << WIFI_BTN_PIN),
        .mode         = GPIO_MODE_INPUT,
        .pull_up_en   = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type    = GPIO_INTR_DISABLE 
    };
    gpio_config(&wifi_btn);
    xTaskCreate(wifi_btn_task, "wifi_control_task", 2048, NULL, 5, NULL);
    
    gpio_config_t pf_pin = {
        .pin_bit_mask = (1ULL << PF_BTN_PIN) |
        (1ULL << WIFI_BTN_PIN),
        .mode         = GPIO_MODE_INPUT,
        .pull_up_en   = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type    = GPIO_INTR_NEGEDGE 
    };
    gpio_config(&pf_pin);

    ESP_ERROR_CHECK(gpio_install_isr_service(0));
    ESP_ERROR_CHECK(gpio_isr_service_add(PF_BTN_PIN, pf_btn_isr, NULL));
    ESP_ERROR_CHECK(gpio_isr_service_add(WIFI_BTN_PIN, wifi_btn_isr, NULL));


    gpio_config_t led_pins = {
        .pin_bit_mask = (1ULL << PF_BTN_PIN) |
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
void gpio_driver_register_pf_callback(gpio_callbacks_t cb_fxn){
    pf_cb = cb_fxn;
}