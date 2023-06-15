# Configurable Options
set(TestDriverName sdio)

# Test Executable
add_executable(TestLLD_STM32F4x_${TestDriverName} test_lld_${TestDriverName}.cpp)
add_test(NAME TestLLD_STM32F4x_${TestDriverName} COMMAND TestLLD_STM32F4x_${TestDriverName})
target_include_directories(TestLLD_STM32F4x_${TestDriverName} PRIVATE ${CPPUTEST_INCLUDE_DIRS})
target_link_directories(TestLLD_STM32F4x_${TestDriverName} PRIVATE ${CPPUTEST_LIBRARIES})
target_link_libraries(TestLLD_STM32F4x_${TestDriverName} PRIVATE ${CPPUTEST_LDFLAGS})

target_link_libraries(TestLLD_STM32F4x_${TestDriverName} PRIVATE
  # Public Includes
#   aurora_intf_inc
#   chimera_intf_inc
  thor_intf_inc

  # Project Libraries
  thor_lld_stm32f4_sdio

  # Target Properties
#   prj_device_target
#   prj_build_target
)
