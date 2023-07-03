include("${COMMON_TOOL_ROOT}/cmake/utility/embedded.cmake")

gen_static_lib_variants(
  TARGET
    thor_cmn_cm4_mock
  SOURCES
    interrupts_expect.cpp 
    interrupts_mock.cpp
  PRV_LIBRARIES
    chimera_intf_inc
    thor_intf_inc
    thor_intf_inc_mock
  EXPORT_DIR
    "${PROJECT_BINARY_DIR}/Thor/lld/common"
)
