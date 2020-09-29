set(PRJ embedded)
add_executable(${PRJ} "${PROJECT_ROOT}/vs/build/Embedded/Embedded/main.cpp")
target_link_libraries(${PRJ} PRIVATE
  # Public Includes
  aurora_inc
  Boost::boost
  chimera_inc
  freertos_inc
  thor_inc
  type_safe_inc

  # Static Libraries
  chimera_src
  freertos_cfg
  freertos_core
  freertos_heap
  freertos_port
  thor_cfg_freertos
  thor_cmn_cm4
  thor_hld
  thor_lld_intf
  thor_lld_stm32l4

  # Target Properties
  prj_device_target
)

# $(OBJCOPY) -O binary --gap-fill 0xFF -S $(>) $(<)
# cmake -DTOOLCHAIN=arm_none_eabi -DFREERTOS_THREADS=ON -DDEVICE_TARGET=stm32l432kb -DCMAKE_BUILD_TYPE=Debug --configure ../


set(TEST_LFS test_little_fs)
add_executable(${TEST_LFS} "${PROJECT_ROOT}/tests/hardware/test_lfs_integration.cpp")
target_link_libraries(${TEST_LFS} PRIVATE
  # Public Includes
  adesto_inc
  aurora_inc
  Boost::boost
  chimera_inc
  freertos_inc
  lfs_inc
  thor_inc
  type_safe_inc

  # Static Libraries
  CppUTest
  adesto_core
  aurora_memory_generic
  chimera_src
  freertos_cfg
  freertos_core
  freertos_heap
  freertos_port
  lib_adesto_at25
  lfs_core
  memory_flash_littlefs
  thor_cfg_freertos
  thor_cmn_cm4
  thor_hld
  thor_lld_intf
  thor_lld_stm32l4

  # Target Properties
  prj_device_target
)

target_compile_definitions(lfs_core PUBLIC LFS_NO_DEBUG LFS_NO_WARN LFS_NO_ERROR LFS_NO_ASSERT)