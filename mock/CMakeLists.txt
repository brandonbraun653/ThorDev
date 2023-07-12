include("${COMMON_TOOL_ROOT}/cmake/utility/embedded.cmake")

add_subdirectory("Thor/lld/common/cortex-m4")
add_subdirectory("Thor/lld/interface/sdio")

gen_intf_lib(
  TARGET
    thor_intf_inc_mock
  INTF_INCLUDES
    "/usr/include"
    "/usr/local/include"
  EXPORT_DIR
    "${PROJECT_BINARY_DIR}/mock"
)