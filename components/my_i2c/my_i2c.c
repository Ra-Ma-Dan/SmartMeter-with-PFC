#include "my_i2c.h"
#include "my_gpio.h"



void i2c_driver_init(){
    i2c_master_bus_config_t bus_cfg = {
        .sda_io_num = I2C_SDA_PIN,
        .scl_io_num = I2C_SCL_PIN,
        .i2c_port   = I2C_PORT,
        .glitch_ignore_cnt = 7,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .flags.enable_internal_pullup = true,
    };
    i2c_new_master_bus(&bus_cfg, &bus_handle);

    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_7,
        .device_address  = I2C_LCD_ADR,
        .scl_speed_hz    = I2C_CLK_HZ
    };
    i2c_master_bus_add_device(bus_handle, &dev_cfg, &dev_handle); 
}


void i2c_driver_send(const uint8_t *data, size_t len_data, uint16_t timeout){
    i2c_master_transmit(dev_handle, data, len_data, timeout);
}
void i2c_driver_recieve(const uint8_t *buffer, size_t len_buf, uint16_t timeout){
    i2c_master_receive(dev_handle, buffer, len_buf, timeout);
}
void i2c_driver_send_recieve(const uint16_t *data, size_t len_data, const uint16_t *buffer, size_t len_buf, uint16_t timeout){
    i2c_master_transmit_receive(dev_handle, data, len_data, buffer, len_buf, timeout);
}
