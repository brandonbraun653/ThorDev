# ====================================================
# Common
# ====================================================
set(LINK_LIBS
  Boost::boost
  chimera_inc       # Chimera public headers
  aurora_inc        # Aurora public headers
  ulog_inc
  project_inc
  aurora_build_options
)

# ====================================================
# Driver Library
# ====================================================
set(LIB thor_test_harness)
add_library(${LIB} STATIC
  blinky.cpp
  logger.cpp
  serial_output.cpp
)
target_link_libraries(${LIB} PRIVATE ${LINK_LIBS} aurora_logging)
export(TARGETS ${LIB} FILE "${PROJECT_BINARY_DIR}/Aurora/${LIB}.cmake")