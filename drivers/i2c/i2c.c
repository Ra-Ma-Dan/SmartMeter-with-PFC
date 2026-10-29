#include "i2c.h"
#include "gpio/gpio.h"



void i2c_driver_init(){
    i2c_master_config_t bus_cfg = {
        .sda_io_num = I2C_SDA_PIN,
        .scl_io_num = I2C_SCL_PIN,
        .flags.enable_internal_pullup = true,
        .i2c_port   = I2C_PORT,
        .glitch_ignore_cnt = 7,
        .clk_source = I2C_CLK_SOURCE_DEFAULT
    };
    i2c_new_master_bus(&bus_handle, &bus_cfg);

    i2c_device_congif_t dev_cfg = {
        .dev_addr_length = I2C_ADR_LEN_BIT_7,
        .device_address  = I2C_LCD_ADR,
        .clk_speed_hz    = I2C_CLK_HZ
    };
    i2c_master_bus_device_add(bus_handle, &dev_handle, &dev_cfg); 
}


void i2c_driver_send(uint16_t data, size_t len_data, uint16_t timeout){
    i2c_master_transmit(dev_handle, data, len_data, timeout);
}
void i2c_driver_recieve(uint16_t buffer, size_t len_buf, uint16_t timeout){
    i2c_master_recieve(dev_handle, buffer, len_buf, timeout);
}
void i2c_driver_send_recieve(uint16_t data, size_t len_data, uint16_t buffer, size_t len_buf, uint16_t timeout){
    i2c_master_transmit_recieve(dev_handle, data, len_data, buffer, len_buf, timeout);
}

void i2c_driver_scan(){

}