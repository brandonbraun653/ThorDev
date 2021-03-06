# -----------------------------------------------------------------------------
# Test Suite: HLD CAN
# Configure Command:
#   cmake -DCMAKE_BUILD_TYPE=Debug -DTOOLCHAIN=gcc -DGTEST=ON -DNATIVE_THREADS=ON -DTHOR_HLD_UNIT_TEST=ON --configure ../
#   cmake -DCMAKE_BUILD_TYPE=Debug -DTOOLCHAIN=gcc -DGTEST=ON -DNATIVE_THREADS=ON -DCOVERAGE=ON -DTHOR_HLD_UNIT_TEST=ON --configure ../
#
# Build Command:
#   cmake --build . --target test_hld_can -j16
#   cmake --build . --target test_hld_can_coverage -j16
# -----------------------------------------------------------------------------
set(TEST_NAME test_hld_can)

# ---------------------------------------------------------
# Coverage Target
# ---------------------------------------------------------
if(COVERAGE)
  include("${PROJECT_ROOT}/lib/CommonTools/cmake/modules/CodeCoverage.cmake")
  setup_target_for_coverage_lcov(
    NAME ${TEST_NAME}_coverage
    EXECUTABLE artifacts/bin/${TEST_NAME}
    DEPENDENCIES ${TEST_NAME}
    BASE_DIRECTORY "${PROJECT_ROOT}"
    EXCLUDE "/usr/*" "${PROJECT_ROOT}/tests/*" "${PROJECT_ROOT}/lib/*" "${PROJECT_ROOT}/Thor/Thor/lld/*"
    NO_DEMANGLE
  )
endif()

# ---------------------------------------------------------
# Executable Target
# ---------------------------------------------------------
add_executable(${TEST_NAME}
  main.cpp
  test_hld_can_chimera.cpp
  test_hld_can_driver.cpp
  test_hld_can_module.cpp
)

target_include_directories(${TEST_NAME} PRIVATE ${PROJECT_ROOT})

target_link_libraries(${TEST_NAME} PRIVATE
  # Public Includes
  gtest_inc
  gmock_inc
  chimera_inc
  aurora_inc
  thor_inc
  Boost::boost
  type_safe_inc
  prj_device_target
  prj_build_target

  # Static Libraries
  gtest_src
  gmock_src
  chimera_src
  thor_hld_can
  thor_lld_mock_can

  # Linux?
  stdc++
  pthread
)

# ---------------------------------------------------------
# Copy Executable Target
# ---------------------------------------------------------
add_custom_command(TARGET ${TEST_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${TEST_NAME}> ${LAST_BUILT_VERSION_DIRECTORY}/test_image)
