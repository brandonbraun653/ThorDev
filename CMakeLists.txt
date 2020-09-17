cmake_minimum_required(VERSION 3.10.0)
project(ThorDev VERSION 0.1.0)
enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# ====================================================
# Override the linker command
# ====================================================
include("lib/CommonTools/cmake/toolchains/linker_whole_archive.cmake")

# ====================================================
# Select which device is being targeted if embedded
# ====================================================
if(Toolchain::HAS_GCC_ARM_NONE_EABI)
  set(LINKER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/CommonTools/linker_scripts/stm32l4x")
  add_subdirectory("lib/CommonTools/cmake/device/stm32l4x")
else()
  set(TARGET_CHIP prj_device_target)
  add_library(${TARGET_CHIP} INTERFACE)
  target_compile_options(${TARGET_CHIP} INTERFACE
    -fdata-sections
    -ffunction-sections
    -fmessage-length=0
    -fno-common
    -fno-exceptions
    $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
  )

  target_link_options(${TARGET_CHIP} INTERFACE
    -Wl,--gc-sections
    -Wl,--print-memory-usage
  )

  target_compile_definitions(${TARGET_CHIP} INTERFACE
    CHIMERA_LITTLE_ENDIAN
  )

  export(TARGETS ${TARGET_CHIP} FILE "${PROJECT_BINARY_DIR}/DeviceTarget/${TARGET_CHIP}.cmake")
endif()

# ====================================================
# Import Boost
# ====================================================
find_package(Boost 1.70.0 EXACT REQUIRED)

# ====================================================
# Import GTest
# ====================================================
add_subdirectory("lib/CommonTools/googletest")

# ====================================================
# Import and Configure FreeRTOS
# ====================================================
if(Toolchain::REQUIRES_THREADING_FREERTOS)
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
endif()

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
if(Toolchain::HAS_GCC_ARM_NONE_EABI)
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
endif()

# $(OBJCOPY) -O binary --gap-fill 0xFF -S $(>) $(<)

if(NOT Toolchain::HAS_GCC_ARM_NONE_EABI)
  add_executable(lld_test Thor/Thor/lld/interface/gpio/test/test_gpio_intf.cpp)
  target_link_libraries(lld_test PRIVATE
    # Public Includes
    gtest_inc
    chimera_inc
    aurora_inc
    thor_inc
    Boost::boost
    type_safe_inc
    prj_device_target

    # Static Libraries
    mingw_stdthreads
    gtest
    chimera_src
    thor_lld_intf
    thor_lld_sim
  )

  target_compile_definitions(prj_device_target INTERFACE
    CHIMERA_LITTLE_ENDIAN
    TARGET_LLD_TEST
    NOWINBASEINTERLOCK
    WIN32_LEAN_AND_MEAN
  )
  target_compile_options(prj_device_target INTERFACE "-m32")
  target_include_directories(prj_device_target INTERFACE "D:/git/Microcontrollers/ThorDev/build/lib/CommonTools/mingw-std-threads-master/cmake_stdheaders_generator/cmake_stdheaders_generator")
endif()