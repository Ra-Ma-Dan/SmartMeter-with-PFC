idf_component_register(
    SRCS "my_gpio.c"
    INCLUDE_DIRS "."
    REQUIRES esp_driver_gpio esp_rom 
)