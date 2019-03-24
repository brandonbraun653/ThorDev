###########################################################
# Global Project Settings/Variables 
###########################################################
# Name of the project/output binary and elf files 
PROJECT = test_uart

# Where this specific project is housed
PROJECT_ROOT = $(CURDIR)/

# Where the build directory will be located 
PROJECT_BUILD_ROOT = $(PROJECT_ROOT)build/

# Full name of STM processor in lowercase 
STM32_PROCESSOR = stm32f767zit

# These binaries must be on the system $PATH
AS 		= arm-none-eabi-as
CC 		= arm-none-eabi-gcc
CPP     = arm-none-eabi-g++
LD 		= arm-none-eabi-g++
OBJCOPY = arm-none-eabi-objcopy
SIZE    = arm-none-eabi-size

# Relevant directories
THOR_ROOT  = $(THOR_PATH)
EIGEN_ROOT = $(EIGEN_PATH)
BOOST_ROOT = $(BOOST_PATH)
STM32_HAL_ROOT = $(STM32F7_HAL_PATH)

# Override settings for the Thor Makefile
USE_FREERTOS  = TRUE
USE_CHIMERA   = FALSE

CPU_FLAGS     = -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16
USER_CFLAGS   = $(CPU_FLAGS)
USER_CXXFLAGS = $(CPU_FLAGS)

include $(THOR_ROOT)/Thor/thor.mk

###########################################################
# Directories 
###########################################################
# Where all the files specific to this project lie
PROJECT_INC_DIRS := $(PROJECT_ROOT)source/ $(PROJECT_ROOT)erpcgen_files/src/ $(ERPC_INC_DIRS) 
PROJECT_SRC_DIRS := $(strip $(PROJECT_ROOT)source/ $(PROJECT_ROOT)erpcgen_files/src/)

PROJECT_DBG_DIR  := $(addprefix $(PROJECT_BUILD_ROOT), debug/)
PROJECT_RLS_DIR  := $(addprefix $(PROJECT_BUILD_ROOT), release/)

$(shell mkdir -p $(PROJECT_DBG_DIR))
$(shell mkdir -p $(PROJECT_RLS_DIR))

# Finds all objects in the release/debug build directories for the linker
RLS_LD_OBJS = $(shell find $(PROJECT_RLS_DIR) -type f -name '*.o')
DBG_LD_OBJS = $(shell find $(PROJECT_DBG_DIR) -type f -name '*.o')

###########################################################
# Files
###########################################################
PROJECT_INC_FILES := $(foreach dir, $(PROJECT_INC_DIRS), $(wildcard $(dir)*.hpp))
PROJECT_SRC_FILES := $(foreach dir, $(PROJECT_SRC_DIRS), $(wildcard $(dir)*.cpp))

# This is the server build, so remove all client files
PROJECT_SRC_FILES := $(filter-out %client.cpp, $(PROJECT_SRC_FILES))


PROJECT_OBJ_FILES = $(patsubst %.cpp, %.o, $(PROJECT_SRC_FILES))


###########################################################
# Build vars for the project
###########################################################
# Generates the expected .o files that should be built
PROJECT_OBJECTS_DBG := $(sort $(addprefix $(PROJECT_DBG_DIR), $(notdir $(PROJECT_OBJ_FILES))))
PROJECT_OBJECTS_RLS := $(sort $(addprefix $(PROJECT_RLS_DIR), $(notdir $(PROJECT_OBJ_FILES))))

# Basic compiler & linker options common to all builds
PROJECT_INCLUDES      = $(addprefix -I, $(PROJECT_INC_DIRS))
PROJECT_CXXFLAGS      = $(CPU_FLAGS) -c -fno-common -fmessage-length=0 -Wall -fno-rtti -fexceptions -ffunction-sections -fdata-sections
PROJECT_CXXDEFS       = -DUSE_FULL_LL_DRIVER -DEIGEN_INITIALIZE_MATRICES_BY_ZERO -D$(STM32_DEVICE_UC) -D$(STM32_DEVICE_FAMILY_UC) 
PROJECT_LDFLAGS       = $(CPU_FLAGS) -Wl,--gc-sections --specs=nano.specs
PROJECT_LD_SYS_LIBS   = -lc -lgcc -lnosys

ifeq ($(USE_FREERTOS), TRUE)
PROJECT_CXXDEFS      += -DUSE_FREERTOS -DUSING_FREERTOS
endif


# On debug target, add these parameters 
debug: PROJECT_CXXFLAGS  += -ggdb -O0
debug: PROJECT_CXXDEFS   += -DDEBUG=1 -DDEBUG_DEFAULT_INTERRUPT_HANDLERS 

# On release target, add these parameters
release: PROJECT_CXXFLAGS  += -O3
release: PROJECT_CXXDEFS   += -DNDEBUG=1 -DRELEASE=1 -DEIGEN_NO_DEBUG

# Tell Make where to find the cpp files
PROJECT_CPP_DIRS := $(sort $(dir $(PROJECT_SRC_FILES)))
vpath %.cpp $(PROJECT_CPP_DIRS)

###########################################################
# Linker Variables
###########################################################
THOR_RLS_DEPS = thor_release $(EIGEN_HEADERS) $(BOOST_HEADERS)
THOR_DBG_DEPS = thor_debug $(EIGEN_HEADERS) $(BOOST_HEADERS)
THOR_CLEAN_DEPS = thor_clean


RELEASE_TARGET = $(PROJECT_RLS_DIR)$(PROJECT)
DEBUG_TARGET = $(PROJECT_DBG_DIR)$(PROJECT)

PROJECT_RELEASE_DEPS = thor_release $(PROJECT_OBJECTS_RLS)
PROJECT_DEBUG_DEPS   = thor_debug $(PROJECT_OBJECTS_DBG)

LINKER_SCRIPT = $(THOR_ROOT)/Thor/device/ld/STM32F767ZI_flash.lds

###########################################################
# Recipes
###########################################################

.PHONY: all release debug clean flash_release flash_debug update_idl
all: release debug 

release: update_idl $(RELEASE_TARGET).bin
	$(call colorecho, $(GREEN), Release build successful)

debug: update_idl $(DEBUG_TARGET).bin
	$(call colorecho, $(GREEN), Debug build successful)

flash_release: release
	st-flash write $(RELEASE_TARGET).bin 0x08000000

flash_debug: debug
	st-flash write $(DEBUG_TARGET).bin 0x08000000

update_idl:
	$(call colorecho, $(BLUE),Updating IDL reference files)
	@mkdir -p ./erpcgen_files/src
	@mkdir -p ./erpcgen_files/py
	
	@erpcgen -g c -o ./erpcgen_files/src/ -v ./erpcgen_files/uart_rpc.erpc 
	@erpcgen -g py -o ./erpcgen_files/py/ -v ./erpcgen_files/uart_rpc.erpc

clean: $(THOR_CLEAN_DEPS)
	@rm -f $(PROJECT_DBG_DIR)*.o $(PROJECT_DBG_DIR)*.bin $(PROJECT_DBG_DIR)*.elf
	@rm -f $(PROJECT_RLS_DIR)*.o $(PROJECT_RLS_DIR)*.bin $(PROJECT_RLS_DIR)*.elf
	$(call colorecho, $(GREEN), Project Build Files Cleaned)

test:
	@echo $(THOR_ROOT)
	

#------------------------------------------
# Generate binaries from the .elf files 
#------------------------------------------
$(RELEASE_TARGET).bin: $(RELEASE_TARGET).elf
	@$(OBJCOPY) -O binary $< $@

$(DEBUG_TARGET).bin: $(DEBUG_TARGET).elf
	@$(OBJCOPY) -O binary $< $@


#------------------------------------------
# Generate .elf files using the linker
#------------------------------------------
# For some reason, the LD_SYS_LIBS need to be linked twice? See: https://goo.gl/xi1h62
$(RELEASE_TARGET).elf: $(PROJECT_RELEASE_DEPS)
	@$(LD) $(PROJECT_LDFLAGS) -T$(LINKER_SCRIPT) -o $@ $(RLS_LD_OBJS) $(PROJECT_LD_SYS_LIBS) $(PROJECT_LD_SYS_LIBS)

$(DEBUG_TARGET).elf: $(PROJECT_DEBUG_DEPS)
	@$(LD) $(PROJECT_LDFLAGS) -T$(LINKER_SCRIPT) -o $@ $(DBG_LD_OBJS) $(PROJECT_LD_SYS_LIBS) $(PROJECT_LD_SYS_LIBS)


#------------------------------------------
# Project specific build commands only
#------------------------------------------
$(PROJECT_RLS_DIR)%.o : %.cpp
	$(call colorecho, $(CYAN), Compiling $(notdir $<) into $@)
	@$(CPP) $(PROJECT_CXXFLAGS) $(USER_CXXFLAGS) $(PROJECT_CXXDEFS) -std=gnu++11 $(PROJECT_INCLUDES) $< -o $@ 

$(PROJECT_DBG_DIR)%.o : %.cpp
	$(call colorecho, $(CYAN), Compiling $(notdir $<) into $@)
	@$(CPP) $(PROJECT_CXXFLAGS) $(USER_CXXFLAGS) $(PROJECT_CXXDEFS) -std=gnu++11 $(PROJECT_INCLUDES) $< -o $@ 