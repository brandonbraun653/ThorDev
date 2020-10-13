# -----------------------------------------------------------------------------
# Test Suite: STM32L4 LLD CAN
# -----------------------------------------------------------------------------
add_executable(test_stm32l4_lld_can
  test_entry.cpp
  test_driver_interface.cpp
)
target_link_libraries(test_stm32l4_lld_can PRIVATE
  # Public Includes
  aurora_inc
  Boost::boost
  chimera_inc
  freertos_inc
  thor_inc
  type_safe_inc

  # Static Libraries
  CppUTest
  aurora_core
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
  prj_build_target
)
target_include_directories(test_stm32l4_lld_can PRIVATE ${PROJECT_ROOT})