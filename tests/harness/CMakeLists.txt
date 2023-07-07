include("${COMMON_TOOL_ROOT}/cmake/utility/embedded.cmake")

gen_static_lib_variants(
  TARGET
    thor_test_harness
  SOURCES
    mbed_test_harness.cpp
  PUB_LIBRARIES
    CppUTest_inc
    aurora_intf_inc
    chimera_intf_inc
    thor_intf_inc
  EXPORT_DIR
    "${PROJECT_BINARY_DIR}/test/harness"
)
