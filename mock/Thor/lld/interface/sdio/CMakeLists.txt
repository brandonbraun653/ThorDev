include("${COMMON_TOOL_ROOT}/cmake/utility/embedded.cmake")

gen_static_lib_variants(
  TARGET
    thor_lld_intf_sdio_mock
  SOURCES
    sdio_driver_expect.cpp
    sdio_intf_expect.cpp
  PRV_LIBRARIES
    chimera_intf_inc
    thor_intf_inc
    thor_intf_inc_mock
  EXPORT_DIR
    "${PROJECT_BINARY_DIR}/mock/Thor/lld/interface/sdio"
)
