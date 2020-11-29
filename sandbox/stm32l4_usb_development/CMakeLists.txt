# -----------------------------------------------------------------------------
# Sandbox for testing out the STM32L4xxxx USB driver development
#
# Configure Command:
#   cmake -DTOOLCHAIN=arm_none_eabi -DFREERTOS_THREADS=ON -DDEVICE_TARGET=stm32l432kb -DCMAKE_BUILD_TYPE=Debug --configure ../
#
# Build Command:
#   cmake --build . --target sandbox_stm32l4_usb
# -----------------------------------------------------------------------------
add_executable(sandbox_stm32l4_usb
  main.cpp
  usb_driver.cpp
)

target_include_directories(sandbox_stm32l4_usb PRIVATE
  "."
  "${PROJECT_ROOT}"
)

target_link_libraries(sandbox_stm32l4_usb PRIVATE
  # Public Includes
  aurora_inc
  Boost::boost
  chimera_inc
  freertos_inc
  thor_inc
  type_safe_inc

  # Static Libraries
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
  thor_test_harness

  # Target Properties
  prj_device_target
  prj_build_target
)
target_include_directories(sandbox_stm32l4_usb PRIVATE ${PROJECT_ROOT})
add_custom_command(TARGET sandbox_stm32l4_usb POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:sandbox_stm32l4_usb> ${LAST_BUILT_VERSION_DIRECTORY}/test_image
)
