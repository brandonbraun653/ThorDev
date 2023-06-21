# Configurable Options
set(TestDriverName sdio)

# Test Executable
add_executable(TestHLD_${TestDriverName} test_hld_${TestDriverName}.cpp)
add_test(NAME TestHLD_${TestDriverName} COMMAND TestHLD_${TestDriverName})
target_include_directories(TestHLD_${TestDriverName} PRIVATE ${CPPUTEST_INCLUDE_DIRS})
target_link_directories(TestHLD_${TestDriverName} PRIVATE ${CPPUTEST_LIBRARIES})
target_link_libraries(TestHLD_${TestDriverName} PRIVATE ${CPPUTEST_LDFLAGS})

target_link_libraries(TestHLD_${TestDriverName} PRIVATE
  # Public Includes
  aurora_intf_inc
  chimera_intf_inc
  thor_intf_inc

  # Project Libraries
  chimera_peripheral_sdio
  chimera_common
)

target_compile_definitions(TestHLD_${TestDriverName} PRIVATE 
	USING_NATIVE_THREADS
	CHIMERA_SIMULATOR
	SIMULATOR
)
