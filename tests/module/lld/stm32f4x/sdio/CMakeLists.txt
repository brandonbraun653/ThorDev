include("${COMMON_TOOL_ROOT}/cmake/utility/embedded.cmake")

set(TestName test_lld_sdio)

# -----------------------------------------------------------------------------
# Project Libraries
# -----------------------------------------------------------------------------
gen_intf_lib_variants(
  TARGET
    embedded_core_libs
  LIBRARIES
    thor_cfg_freertos
    thor_cmn_cm4
    thor_hld_adc
    thor_hld_can
    thor_hld_clock
    thor_hld_dma
    thor_hld_exti
    thor_hld_gpio
    thor_hld_i2c
    thor_hld_interrupt
    thor_hld_serial
    thor_hld_spi
    thor_hld_system
    thor_hld_timer
    thor_hld_uart
    thor_hld_usart
    thor_hld_watchdog
    thor_lld_cmn
    thor_lld_intf_adc
    thor_lld_intf_can
    thor_lld_intf_dma
    thor_lld_intf_exti
    thor_lld_intf_gpio
    thor_lld_intf_i2c
    thor_lld_intf_interrupt
    thor_lld_intf_rcc
    thor_lld_intf_sdio
    thor_lld_intf_serial
    thor_lld_intf_spi
    thor_lld_intf_sys
    thor_lld_intf_timer
    thor_lld_intf_usart
    thor_lld_intf_watchdog
  EXPORT_DIR
    ${PROJECT_BINARY_DIR}/${TestName}
)

gen_intf_lib_variants(
  TARGET
    stm32f4_libs
  LIBRARIES
    thor_lld_stm32f4_adc
    thor_lld_stm32f4_can
    thor_lld_stm32f4_des
    thor_lld_stm32f4_dma
    thor_lld_stm32f4_exti
    thor_lld_stm32f4_flash
    thor_lld_stm32f4_gpio
    thor_lld_stm32f4_i2c
    thor_lld_stm32f4_interrupt
    thor_lld_stm32f4_iwdg
    thor_lld_stm32f4_power
    thor_lld_stm32f4_rcc
    thor_lld_stm32f4_sdio
    thor_lld_stm32f4_spi
    thor_lld_stm32f4_startup
    thor_lld_stm32f4_system
    thor_lld_stm32f4_usart
    thor_lld_stm32f4_wwdg
  EXPORT_DIR
    ${PROJECT_BINARY_DIR}/${TestName}
)

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
  CppUTest
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
