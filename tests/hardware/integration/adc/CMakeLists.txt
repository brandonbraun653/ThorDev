# -----------------------------------------------------------------------------
# Test Suite: HLD ADC
#
# cmake -DTOOLCHAIN=arm-none-eabi -DFREERTOS_THREADS=ON -DDEVICE_TARGET=stm32l432kb -DCMAKE_BUILD_TYPE=Debug --configure ../
# -----------------------------------------------------------------------------
add_executable(test_hld_adc_hw
  test_hld_adc_hw.cpp
)

target_include_directories(test_hld_adc_hw PRIVATE "." )

target_link_libraries(test_hld_adc_hw PRIVATE
  # Public Includes
  aurora_inc
  Boost::boost
  chimera_inc
  freertos_inc
  thor_inc
  type_safe_inc

  # Static Libraries
  aurora_core
  aurora_logging
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
  thor_test_harness

  # Target Properties
  prj_device_target
  prj_build_target
)
target_include_directories(test_hld_adc_hw PRIVATE ${PROJECT_ROOT})
add_custom_command(TARGET test_hld_adc_hw POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:test_hld_adc_hw> ${LAST_BUILT_VERSION_DIRECTORY}/test_image
)
