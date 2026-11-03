idf_component_register(SRC "my_uart.c"
                        PREV_REQIRES esp_driver_uart gpio uart 
                        INCLUDE_DIRS ".")