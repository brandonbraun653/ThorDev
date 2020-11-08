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
# Set some vars common to both development environments
include("lib/CommonTools/cmake/options/common.cmake")

# Override the linker command
if(Toolchain::HAS_GCC_ARM_NONE_EABI)
  include("lib/CommonTools/cmake/toolchains/linker_whole_archive.cmake")
endif()

# ====================================================
# Import common packages and projects
# ====================================================
find_package(Boost REQUIRED)
add_subdirectory("lib/CommonTools/googletest")
add_subdirectory("lib/CommonTools/cpputest")
add_subdirectory("lib/Aurora")
add_subdirectory("lib/Chimera")
add_subdirectory("lib/FreeRTOS")
add_subdirectory("lib/FlashMemory")
add_subdirectory("Thor")

# ====================================================
# Import test targets
# ====================================================
# Hardware tests execute on a physical embedded target
add_subdirectory("tests/hardware/harness")
add_subdirectory("tests/hardware/peripherals")
add_subdirectory("tests/hardware/integration")

# Software tests run on a PC and are hardware agnostic
add_subdirectory("tests/software/can")

# ====================================================
# Project Exports
# ====================================================
# Public include location
add_library(project_inc INTERFACE)
target_include_directories(project_inc INTERFACE "${PROJECT_ROOT}")
export(TARGETS project_inc FILE "${PROJECT_BINARY_DIR}/Project/project_inc.cmake")