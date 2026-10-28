#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/i2c_master.h"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_wifi.h"
#include "nvs_flash.h"



//i2c port pins
#define SDA_PIN      GPIO_NUM_37
#define SCL_PIN      GPIO_NUM_38
#define DEVICE_ADR   0x23
#define CLK_SPEED    400000

bool is_wifi_on = true;
bool is_PFC_on  = true;





void i2c_cfg(){
    i2c_master_bus_handle_t bus_handle;
    i2c_master_config_t bus_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .scl_io_num = SCL_PIN,
        .sda_io_num = SDA_PIN,
        .flags.enable_internal_pullup = true,
        .i2c_port   = TEST_I2C_PORT
    };
    i2c_new_master_bus(&bus_handle, &bus_config);

    i2c_master_dev_handle_t dev_handle;
    i2c_device_donfig_t dev_config = {
        .dev_addr_length = I2C_DEV_BIT_LEN_7,
        .device_address  = DEVICE_ADR,
        .clk_speed_hz    = CLK_SPEED
    };
    i2c_master_bus_device_add(bus_handle, &dev_handle, &dev_config);

    ESP_LOGI(TAG, "I2C bus initialized");
}

void wifi_task(){
    while(true){
        if(is_wifi_on){
            wifi_on
        }
    }
}
void app_main() {
  printf("Hello, Wokwi!\n");
  while (true) {
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
