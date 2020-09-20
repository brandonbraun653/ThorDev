set(PRJ embedded)
add_executable(${PRJ} ../vs/build/Embedded/Embedded/main.cpp)
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
# cmake -DTOOLCHAIN=arm_none_eabi -DLIB_FREERTOS=ON -DDEVICE_TARGET=stm32l432kb --configure ../