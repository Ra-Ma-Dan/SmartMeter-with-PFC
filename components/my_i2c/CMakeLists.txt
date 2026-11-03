idf_component_register(
    SRCS "my_i2c.c"
    INCLUDE_DIRS "."
    REQUIRES esp_driver_i2c my_gpio
)
