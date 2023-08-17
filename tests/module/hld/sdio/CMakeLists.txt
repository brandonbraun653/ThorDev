# Configurable Options
set(TestDriverName test_hld_sdio)

# Test Executable
add_executable(${TestDriverName} ${TestDriverName}.cpp)
add_test(NAME ${TestDriverName} COMMAND ${TestDriverName})
target_include_directories(${TestDriverName} PRIVATE ${CPPUTEST_INCLUDE_DIRS})
target_link_directories(${TestDriverName} PRIVATE ${CPPUTEST_LIBRARIES})
target_link_libraries(${TestDriverName} PRIVATE ${CPPUTEST_LDFLAGS})

target_link_libraries(${TestDriverName} PRIVATE
  # Public Includes
  aurora_intf_inc
  chimera_intf_inc
  thor_intf_inc
  thor_intf_inc_mock

  # Library Under Test
  thor_hld_sdio

  # Project Libraries
  thor_lld_intf_sdio_mock
  chimera_core
  aurora_logging
)

target_compile_definitions(${TestDriverName} PRIVATE
	USING_NATIVE_THREADS
	CHIMERA_SIMULATOR
	SIMULATOR
)
