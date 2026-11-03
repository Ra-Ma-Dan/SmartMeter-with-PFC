#ifndef MY_UART_H
#define MY_UART_H

#include "freertos/queue.h"

#define UART_PORT       UART_NUM_0
#define UART_BOD_RATE      9600

QueueHandle_t uart_queue;
const int uart_buffer_size = (1024 * 2);
int timeout_ms = 100;

void uart_driver_init();



#endif