#include <stdio.h>
#include <string.h>
#include "drivers/uart.h"
#include "esp_log.h"
#include "esp_err.h"
#include "uart.h"
#include "gpio/gpio.h"


static const char *TAG = "UART Driver";


void uart_driver_init(){
    esp_err_t res;

    uart_config_t uart_config = {
        .baud_rate = UART_BOD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_CTS_RTS,
        .rx_flow_ctrl_thresh = 122,
    };

    // Configure UART parameters
    res = uart_param_config(UART_PORT, &uart_config);
    if (res != ESP_OK){
        ESP_LOGE(TAG, "Failed config UArt bus %s", err_to_name(res));
    }

    res = uart_set_pin(UART_PORT, UART_TX_PIN, UART_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    if (res != ESP_OK){
        ESP_LOGE(TAG, "Failed config UArt bus %s", err_to_name(res));
    }

    res = uart_driver_install(UART_NUM_2, uart_buffer_size, uart_buffer_size, 10, &uart_queue, 0);
    if (res != ESP_OK){
        ESP_LOGE(TAG, "Failed config UArt bus %s", err_to_name(res));
    }
}

