# Root CMakeLists.txt
cmake_minimum_required(VERSION 3.16)

# Tell ESP-IDF where your custom components are located
set(EXTRA_COMPONENT_DIRS 
    "drivers/gpio"
    "drivers/i2c"
    "drivers/nvs"
    "drivers/uart"
    "drivers/wifi"
    "peripherals/lcd_display"
    "peripherals/pmez"
    "server/http_server"
)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(PFC_Meter_project)
