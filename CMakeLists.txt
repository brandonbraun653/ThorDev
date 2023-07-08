cmake_minimum_required(VERSION 3.18.0)
project(ThorDev VERSION 0.1.0)
enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(CPP_PLATFORM "freertos")

# ====================================================
# Perform very high level configuration
# ====================================================
include(Dart)
# Set some vars common to both development environments
include("lib/CommonTools/cmake/options/common.cmake")

# Override the linker command
if(Toolchain::HAS_GCC_ARM_NONE_EABI)
  include("lib/CommonTools/cmake/toolchains/linker_whole_archive.cmake")
endif()

# ====================================================
# Import common packages and projects
# ====================================================
if(NOT Toolchain::HAS_GCC_ARM_NONE_EABI)

  # This is a bit kludgy, but it works for now. I'll need to figure out a better way to do this
  add_subdirectory("Thor/lld/stm32f4x")

  if(DEFINED ENV{CPPUTEST_HOME})
      message(STATUS "Using CppUTest home: $ENV{CPPUTEST_HOME}")
      set(CPPUTEST_INCLUDE_DIRS $ENV{CPPUTEST_HOME}/include)
      set(CPPUTEST_LIBRARIES $ENV{CPPUTEST_HOME}/lib)
      set(CPPUTEST_LDFLAGS CppUTest CppUTestExt)
  else()
      find_package(PkgConfig REQUIRED)
      pkg_search_module(CPPUTEST REQUIRED cpputest>=3.8)
      message(STATUS "Found CppUTest version ${CPPUTEST_VERSION}")
  endif()
endif()

add_subdirectory("Thor")
add_subdirectory("lib/Aurora")
add_subdirectory("lib/Chimera")
add_subdirectory("lib/CommonTools/cpputest")
add_subdirectory("lib/FreeRTOS")

# ====================================================
# Import test targets
# ====================================================
add_subdirectory("tests")
add_subdirectory("mock")

# ====================================================
# Project Exports
# ====================================================
# Project Headers
gen_intf_lib(
  TARGET
    project_intf_inc
  INTF_DEFINES
    THOR_USER_CONFIG
  INTF_INCLUDES
    "${PROJECT_ROOT}"
    "${PROJECT_ROOT}/tests/config/bsp"
    "${PROJECT_ROOT}/tests/config/cpputest"
    "${PROJECT_ROOT}/tests/config/freertos"
    "${PROJECT_ROOT}/tests/config/thor"
  INTF_LIBRARIES
    #freertos_intf_inc_all
  EXPORT_DIR
    "${PROJECT_BINARY_DIR}/Project"
)

# Need to figure out how to switch this based on the target
target_compile_definitions(project_intf_inc INTERFACE STM32F446xx TARGET_STM32F4)

# FreeRTOS Configuration
gen_intf_lib(
  TARGET
    freertos_cfg
  INTF_DEFINES
    #SEGGER_SYS_VIEW
  INTF_INCLUDES
    "${PROJECT_ROOT}/tests/config/freertos"
    "${PROJECT_ROOT}/lib/Aurora/lib/segger/contrib/cortex_m4_freertos"
  INTF_LIBRARIES
    segger_sys_view_intf
    segger_sys_view_cortex_m4_freertos_intf
  EXPORT_DIR
    "${PROJECT_BINARY_DIR}/Project"
)

gen_intf_lib(
  TARGET
    freertos_port
  INTF_INCLUDES
    "${PROJECT_ROOT}/lib/FreeRTOS/portable/GCC/ARM_CM4F"
  INTF_DEFINES
    FREERTOS_CFG_PORT_ARM_CM4F=1
  EXPORT_DIR
    "${PROJECT_BINARY_DIR}/Project"
)

gen_intf_lib(
  TARGET
    freertos_heap
  INTF_DEFINES
    FREERTOS_CFG_MEM_MANG_HEAP4=1
  EXPORT_DIR
    "${PROJECT_BINARY_DIR}/Project"
)