cmake_minimum_required(VERSION 3.10.0)
project(ThorDev VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ====================================================
# Select which device is being targeted
# ====================================================
add_subdirectory("lib/CommonTools/cmake/device/stm32l4")

# ====================================================
# Import and Configure FreeRTOS
# ====================================================
include("lib/CommonTools/cmake/options/freertos.cmake")
add_subdirectory("lib/FreeRTOS")

# FreeRTOS Config Options
add_library(freertos_cfg INTERFACE)
target_include_directories(freertos_cfg INTERFACE "Thor/Thor/config/freertos")
export(TARGETS freertos_cfg FILE "${PROJECT_BINARY_DIR}/ProjectConfig/freertos-cfg.cmake")

# FreeRTOS Port Selection
add_library(freertos_port INTERFACE)
target_link_libraries(freertos_port INTERFACE freertos_cm4f)
export(TARGETS freertos_port FILE "${PROJECT_BINARY_DIR}/ProjectConfig/freertos-port.cmake")

# FreeRTOS Heap Selection
add_library(freertos_heap INTERFACE)
target_link_libraries(freertos_heap INTERFACE freertos_heap4)
export(TARGETS freertos_heap FILE "${PROJECT_BINARY_DIR}/ProjectConfig/freertos-heap.cmake")

# ====================================================
# Import and Configure Chimera
# ====================================================
add_subdirectory("lib/Aurora/Chimera")

# ====================================================
# Import and Configure Aurora
# ====================================================
add_subdirectory("lib/Aurora")

# ====================================================
# Import and Configure Thor
# ====================================================
include("lib/CommonTools/cmake/options/thor.cmake")
add_subdirectory("Thor")

# ====================================================
# Project Primary Target
# ====================================================
# add_library(FreeRTOSDev STATIC FreeRTOSDev.cpp)
# target_link_libraries(FreeRTOSDev PRIVATE
#   freertos_inc
#   freertos_core
#   freertos_cfg
#   freertos_port
#   freertos_heap
#   prj_device_target
# )
