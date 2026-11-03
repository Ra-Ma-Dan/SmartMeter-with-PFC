idf_component_register(SRC "my_uart.c"
                        PREV_REQIRES esp_driver_uart my_gpio 
                        INCLUDE_DIRS ".")