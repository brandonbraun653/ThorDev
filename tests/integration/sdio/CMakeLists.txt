include("${COMMON_TOOL_ROOT}/cmake/utility/embedded.cmake")

set(TestName sdio_integration_test)

# -----------------------------------------------------------------------------
# Project Libraries
# -----------------------------------------------------------------------------
add_executable(${TestName} ${TestName}.cpp)
target_include_directories(${TestName} PRIVATE ".")
target_include_directories(${TestName} PRIVATE ${PROJECT_ROOT})
target_link_libraries(${TestName} PRIVATE
  # Public Includes
  aurora_intf_inc
  chimera_intf_inc
  freertos_intf_inc_all
  thor_intf_inc
  CppUTest_inc

  # Project Libraries
  CppUTest_rel
  aurora_core
  aurora_logging
  chimera_core
  embedded_core_libs
  stm32f4_libs
  freertos_intf_inc_all
  freertos_core
  freertos_port
  freertos_heap
  freertos_heap_4
  freertos_cm4f_port
  thor_test_harness

  # Target Properties
  prj_device_target
  prj_build_target
)

add_custom_command(TARGET ${TestName} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${TestName}> ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/mbed_test_driver
)
