cmake_minimum_required(VERSION 3.16)

# Point to the top-level folders. ESP-IDF will automatically find all components inside them!
set(EXTRA_COMPONENT_DIRS "components" "peripherals" "server") 

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(PFC_Meter_project)
