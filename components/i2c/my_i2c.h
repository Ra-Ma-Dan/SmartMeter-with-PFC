#ifndef MY_I2C_H
#define MY_I2C_H

#include "driver/i2c_master.h"




#define I2C_PORT     TEST_I2C_PORT
#define I2C_CLK_HZ   100000
#define I2C_LCD_ADR  0x27   //LCD screen address

i2c_master_bus_handle_t bus_handle;
i2c_master_dev_handle_t dev_handle;


void i2c_driver_init();
void i2c_driver_send(uint16_t data, size_t len_data, uint16_t timeout);
void i2c_driver_recieve(uint16_t buffer, size_t len_buf, uint16_t timeout);
void i2c_driver_send_recieve(uint16_t data, size_t len_data, uint16_t buffer, size_t len_buf, uint16_t timeout);


#endif