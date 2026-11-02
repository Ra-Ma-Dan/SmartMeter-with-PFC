#include "pmez.h"
#include "driver/uart.h"
#include "gpio/gpio.h"

#define PZEM_UART_NUM      UART_NUM_2  // Best choice on ESP32


// Modbus Command to read input registers 0x0000 to 0x000A
const uint8_t pzem_read_cmd[] = {0x01, 0x04, 0x00, 0x00, 0x00, 0x0A, 0x70, 0x0D};

void pmez_init(void) {
    
    //uart_driver_install(PZEM_UART_NUM, 1024 * 2, 0, 0, NULL, 0);
   
    void uart_driver_init();
}

bool pmez_read_data(pzem_data_t *data) {
    // 1. Send the Modbus request packet over UART
    uart_write_bytes(PZEM_UART_NUM, (const char*)pzem_read_cmd, sizeof(pzem_read_cmd));
    
    uint8_t response[25];
    // 2. Read the 25-byte response buffer back from the chip
    int len = uart_read_bytes(PZEM_UART_NUM, response, sizeof(response), pdMS_TO_TICKS(200));
    
    if (len >= 25 && response[0] == 0x01 && response[1] == 0x04) {
        // Parse registers (High byte << 8 | Low byte)
        data->voltage = ((response[3] << 8) | response[4]) * 0.1f;
        data->current = ((response[5] << 8) | response[6]) * 0.001f;
        data->power   = ((response[7] << 24) | (response[8] << 16) | (response[9] << 8) | response[10]) * 0.1f;
        data->power_factor = ((response[19] << 8) | response[20]) * 0.01f;
        
        return true;
    }
    return false; // Timeout or bad packet
}
