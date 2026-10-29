#ifndef GPIO_H
#define GPIO_H

#include "driver/gpio.h"


//Indicators and BTN pins
#define PF_BTN_PIN    GPIO_NUM_12
#define WIFI_BTN_PIN  GPIO_NUM_19
#define Credit_LED    GPIO_NUM_13
#define WIFI_LED_PIN  GPIO_NUM_17
#define PF_LED_PIN    GPIO_NUM_18
#define POWER_LED     GPIO_NUM_20

//Relay pins for Capacitors' controls
#define RELAY_1_PIN  GPIO_NUM_21
#define RELAY_2_PIN  GPIO_NUM_15
#define RELAY_3_PIN  GPIO_NUM_16
#define RELAY_4_PIN  GPIO_NUM_14

//UART configuration params
#define UART_TX_PIN  GPIO_NUM_37
#define UART_RX_PIN  GPIO_NUM_38
#define CLK_SPEED    19600

//I2C configuration parameters
#define I2C_SDA_PIN  GPIO_NUM_6
#define I2C_SCL_PIN  GPIO_NUM_7

void gpio_pins_config();


#endif

