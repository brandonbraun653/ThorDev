cmake_minimum_required(VERSION 3.10.0)

# ====================================================
# Override the toolchain used. This must be done here
# else CMake won't allow the override.
#
# The paths are slightly different due to weird handling
# in Windows vs Linux. I haven't found out why that occurs.
# ====================================================
if(WIN32)
  set(PROJECT_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/.." CACHE STRING "")
  set(COMMON_TOOL_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../lib/CommonTools" CACHE STRING "")
else()
  set(PROJECT_ROOT "${CMAKE_CURRENT_SOURCE_DIR}" CACHE STRING "")
  set(COMMON_TOOL_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/lib/CommonTools" CACHE STRING "")
endif()

include("${COMMON_TOOL_ROOT}/cmake/options/toolchain.cmake")