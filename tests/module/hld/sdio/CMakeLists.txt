# Configurable Options
set(TestDriverName sdio)

# Test Executable
add_executable(TestHLD_${TestDriverName} test_hld_${TestDriverName}.cpp)
add_test(NAME TestHLD_${TestDriverName} COMMAND TestHLD_${TestDriverName})
target_include_directories(TestHLD_${TestDriverName} PRIVATE ${CPPUTEST_INCLUDE_DIRS})
target_link_directories(TestHLD_${TestDriverName} PRIVATE ${CPPUTEST_LIBRARIES})
target_link_libraries(TestHLD_${TestDriverName} PRIVATE ${CPPUTEST_LDFLAGS})

# target_link_libraries(TestHLD_${TestDriverName} PRIVATE
#   # Public Includes
#   aurora_intf_inc
#   chimera_intf_inc
#   thor_intf_inc

#   # Project Libraries
#   aurora_libs
#   chimera_core

#   # Target Properties
#   prj_device_target
#   prj_build_target
# )

# Hardware Driver Model
# if(${DEVICE_TARGET} MATCHES "^stm32[lf]4")
#   target_link_libraries(TestHLD_${TestDriverName} PRIVATE embedded_core_libs)

#   if(${DEVICE_TARGET} MATCHES "^stm32l4")
#     target_link_libraries(TestHLD_${TestDriverName} PRIVATE stm32l4_libs)
#   elseif(${DEVICE_TARGET} MATCHES "^stm32f4")
#     target_link_libraries(TestHLD_${TestDriverName} PRIVATE stm32f4_libs)
#   endif()

# elseif(${DEVICE_TARGET} MATCHES "^sim")
#   target_link_libraries(TestHLD_${TestDriverName} PRIVATE
#     chimera_sim_core
#     gtest_src
#     gmock_src
#     zmq
#     )
# else()
#   message(FATAL_ERROR "Unsupported device target")
# endif()