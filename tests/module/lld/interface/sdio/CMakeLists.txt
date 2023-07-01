# Configurable Options
set(TestDriverName sdio)

# Test Executable
add_executable(TestLLD_Intf_${TestDriverName} test_lld_intf_${TestDriverName}.cpp)
add_test(NAME TestLLD_Intf_${TestDriverName} COMMAND TestLLD_Intf_${TestDriverName})
target_include_directories(TestLLD_Intf_${TestDriverName} PRIVATE ${CPPUTEST_INCLUDE_DIRS})
target_link_directories(TestLLD_Intf_${TestDriverName} PRIVATE ${CPPUTEST_LIBRARIES})
target_link_libraries(TestLLD_Intf_${TestDriverName} PRIVATE ${CPPUTEST_LDFLAGS})

target_link_libraries(TestLLD_Intf_${TestDriverName} PRIVATE
  # Public Includes
  aurora_intf_inc
  chimera_intf_inc
  thor_intf_inc

  # Project Libraries
  thor_lld_intf_sdio
  chimera_common

  # Target Properties
  prj_device_target
  prj_build_target
)
