cmake_minimum_required(VERSION 3.10.0)


# Configure the output generator type
#set(CMAKE_GENERATOR "Unix Makefiles" CACHE INTERNAL "" FORCE)

# ====================================================
# Configure the available toolchains
# ====================================================
set(COMMON_TOOL_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/lib/CommonTools")
set(ARM_NONE_EABI_ROOT "")
set(GCC_BIN_ROOT "D:/ProgramFiles/TDM-GCC-64/bin")

include("lib/CommonTools/cmake/options/common.cmake")
include("lib/CommonTools/cmake/options/toolchain.cmake")
include("lib/CommonTools/cmake/options/validator.cmake")