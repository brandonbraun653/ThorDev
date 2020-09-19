set(TARGET_CHIP prj_device_target)
add_library(${TARGET_CHIP} INTERFACE)
target_compile_options(${TARGET_CHIP} INTERFACE
  -fdata-sections
  -ffunction-sections
  -fmessage-length=0
  -fno-common
  -fno-exceptions
  -m64
  $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
)

target_link_options(${TARGET_CHIP} INTERFACE
  -Wl,--gc-sections
)

target_compile_definitions(${TARGET_CHIP} INTERFACE
  CHIMERA_LITTLE_ENDIAN
)

export(TARGETS ${TARGET_CHIP} FILE "${PROJECT_BINARY_DIR}/DeviceTarget/${TARGET_CHIP}.cmake")


add_executable(lld_test Thor/Thor/lld/interface/gpio/test/test_gpio_intf.cpp)
target_link_libraries(lld_test PRIVATE
  # Public Includes
  gtest_inc
  chimera_inc
  aurora_inc
  thor_inc
  Boost::boost
  type_safe_inc
  prj_device_target

  # Static Libraries
  gtest_src
  gtest_main
  chimera_src
  thor_lld_intf
  thor_lld_sim

  # Linux?
  stdc++
  pthread
)

target_compile_definitions(prj_device_target INTERFACE
  CHIMERA_LITTLE_ENDIAN
  TARGET_LLD_TEST
)

# cmake -DTOOLCHAIN=gcc -DGTEST=ON -DNATIVE_THREADS=ON --configure ../