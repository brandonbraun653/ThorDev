# Configurable Options
set(TestDriverName test_lld_intf_sdio)

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

  # Project Libraries
  thor_lld_intf_sdio
  chimera_common

  # Target Properties
  prj_device_target
  prj_build_target
)

# TODO: Need to figure out how to get the correct LLD for the target
target_link_libraries(${TestDriverName} PRIVATE thor_lld_stm32f4_sdio)

add_custom_command(TARGET ${TestDriverName} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${TestDriverName}> ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_app
)
