idf_component_register(SRC "i2c.c"
                        PREV_REQIRES driver/i2c_master.h gpio/gpio.h
                        INCLUDE_DIRS ".")