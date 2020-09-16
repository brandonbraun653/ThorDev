cmake_minimum_required(VERSION 3.10.0)
project(ThorDev VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ====================================================
# Override the linker command
# ====================================================
include("lib/CommonTools/cmake/toolchains/linker_whole_archive.cmake")

# ====================================================
# Select which device is being targeted
# ====================================================
set(LINKER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/CommonTools/linker_scripts/stm32l4x")
add_subdirectory("lib/CommonTools/cmake/device/stm32l4x")

# ====================================================
# Add Boost
# ====================================================
find_package(Boost 1.70.0 EXACT REQUIRED)

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
add_subdirectory("lib/Chimera")

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
set(PRJ embedded)
add_executable(${PRJ}.elf vs/build/Embedded/Embedded/main.cpp)
target_link_libraries(${PRJ}.elf PRIVATE
  # Public Includes
  aurora_inc
  Boost::boost
  chimera_inc
  freertos_inc
  thor_inc
  type_safe_inc

  # Static Libraries
  chimera_src
  freertos_cfg
  freertos_core
  freertos_heap
  freertos_port
  thor_cfg_freertos
  thor_cmn_cm4
  thor_hld
  thor_lld_intf
  thor_lld_stm32l4

  # Target Properties
  prj_device_target
)

# $(OBJCOPY) -O binary --gap-fill 0xFF -S $(>) $(<)