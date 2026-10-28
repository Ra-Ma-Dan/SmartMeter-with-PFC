#include "gpio.h"


static void IRAM_ATTR pf_btn_isr(void *pvParameters){

}

static void IRAM_ATTR wifi_btn_isr(void *pvParameters){

}

void gpio_pins_config(){
    gpio_config_t input_pins = {
        .pin_bit_mask = (1ULL << PF_BTN_PIN) |
                        (1ULL << WIFI_BTN_PIN),
        .mode         = GPIO_MODE_INPUT,
        .pull_up_en   = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type    = GPIO_INTR_NEGEDGE 
    };
    gpio_config(&input_pins);

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