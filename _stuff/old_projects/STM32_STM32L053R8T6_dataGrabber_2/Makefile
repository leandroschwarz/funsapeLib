# ------------------------------------------------
# AVR 8 Makefile essentials
# ------------------------------------------------

#######################################
# Recursive wildcards
#######################################

rwildcard			= $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))
filter-out-any		= $(foreach v,$(2),$(if $(findstring $(1),$(v)),,$(v)))
filter-in-any		= $(foreach v,$(2),$(if $(findstring $(1),$(v)),$(v),))

#######################################
# Default values
#######################################

# General variables
BUILD_DIR			?= build
CORE_FLOAT-ABI		?=
CORE_FPU			?=
CORE_ID				?= cortex-m0plus
EXCLUDE_PATHS		?= Templates/ STM32L0xx_HAL_Driver/ CMSIS/
FUNSAPE_PATH		?=
TARGET				?= dataGrabber
USE_DEBUG			?= 1

# Compiler and Linker specific variables
DEFS_ASM			?=
DEFS_CXX			?=
GCC_PATH			?=
GCC_PREFIX			?= arm-none-eabi-
LDSCRIPT			?= STM32L053R8Tx_FLASH.ld
OPTIMIZATION		?= -Og
STD					?= c++11

# Programmer specific variables
# AVRDUDE_CONFIG_FILE	?= "C:\\Program Files (x86)\\avrdudess\\avrdude.conf"
# BAUD				?= 57600
# PORT				?= COM5
# PROGRAMMER			?= arduino

#######################################
# Paths
#######################################

HEADER_HXX_PATHS	:=  \
Inc \
Drivers/STM32L0xx_HAL_Driver/Inc \
Drivers/STM32L0xx_HAL_Driver/Inc/Legacy \
Drivers/CMSIS/Device/ST/STM32L0xx/Include \
Drivers/CMSIS/include

######################################
# FUNSAPE Library
######################################

######################################
# Files
######################################

SOURCES_AXX_FILES	 =  \
startup_stm32l053xx.s

SOURCES_C_FILES =  \
Src/stm32l0xx_it.c \
Src/stm32l0xx_hal_msp.c \
Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_adc.c 
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_adc_ex.c \
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_i2c.c \
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_i2c_ex.c \
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_spi.c \
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_tim.c \
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_tim_ex.c \
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_uart.c \
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_uart_ex.c \
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal.c \
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_rcc.c \
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_rcc_ex.c \
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_flash_ramfunc.c \
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_flash.c \
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_flash_ex.c \
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_gpio.c \
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_dma.c \
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_pwr.c \
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_pwr_ex.c \
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_cortex.c \
# Src/system_stm32l0xx.c

SOURCES_CPP_FILES	= \
Src/main.cpp

SOURCES_CXX_FILES	:= $(sort $(SOURCES_C_FILES) $(SOURCES_CPP_FILES))

#######################################
# Executable paths
#######################################

ifeq ($(GCC_PATH), )
	AS				:=
	CC				:=
	CCPP			:=
	LD				:=
	OBJDUMP			:=
	OBJCOPY			:=
	SIZE			:=
else
	AS				:= $(GCC_PATH)/
	CC				:= $(GCC_PATH)/
	CCPP			:= $(GCC_PATH)/
	LD				:= $(GCC_PATH)/
	OBJDUMP			:= $(GCC_PATH)/
	OBJCOPY			:= $(GCC_PATH)/
	SIZE			:= $(GCC_PATH)/
endif
AS					+= $(GCC_PREFIX)gcc -x assembler-with-cpp
CC					+= $(GCC_PREFIX)gcc
CCPP				+= $(GCC_PREFIX)g++
LD					+= $(GCC_PREFIX)g++
OBJDUMP				+= $(GCC_PREFIX)objdump
OBJCOPY				+= $(GCC_PREFIX)objcopy
SIZE				+= $(GCC_PREFIX)size
# OBJCOPY = CP
# SIZE = SZ
HEX = $(OBJCOPY) -O ihex
BIN = $(OBJCOPY) -O binary -S

#######################################
# Compiler flags
#######################################

# Include paths flags
INCLUDES_ASM		:= $(addprefix -I, $(HEADER_ASM_PATHS))
INCLUDES_HXX		:= $(addprefix -I, $(HEADER_HXX_PATHS))

# Device standard flags
FLAG_MCU			:= -mcpu=$(CORE_ID) -mthumb $(FPU) $(FLOAT-ABI)
FLAG_STD			:= -std=$(STD)
FLAG_OPT			:= $(OPTIMIZATION)

# Compiler warnings
FLAG_ASM_WARN		:= -Wall -fdata-sections -ffunction-sections
FLAG_CXX_WARN		:= -Wall -fdata-sections -ffunction-sections

# Assembler flags
FLAGS_ASM			:= $(FLAG_MCU)
FLAGS_ASM			+= $(DEFS_ASM)
FLAGS_ASM			+= -I. $(INCLUDES_ASM)
FLAGS_ASM			+= $(FLAG_OPT)
FLAGS_ASM			+= $(FLAG_ASM_WARN)

# Compiler flags
FLAGS_CXX			:= $(FLAG_MCU)
FLAGS_CXX			+= -I. $(INCLUDES_HXX)
FLAGS_CXX			+= $(DEFS_CXX)
FLAGS_CXX			+= $(FLAG_OPT)
FLAGS_CXX			+= $(FLAG_STD)
FLAGS_CXX			+= $(FLAG_CXX_WARN)
ifeq ($(USE_DEBUG), 1)
	FLAGS_CXX		+= -g -gdwarf-2
endif
FLAGS_CXX			+= -MMD -MP -MF"$(@:%.o=%.d)"

#######################################
# Linker flags
#######################################

# Libraries
LIBS				:= -lc -lm -lnosys
LIBDIR				:=
LDFLAGS				:= $(FLAG_MCU)
LDFLAGS				+= -specs=nano.specs
LDFLAGS				+= -T$(LDSCRIPT)
LDFLAGS				+= $(LIBDIR)
LDFLAGS				+= $(LIBS)
LDFLAGS				+= -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

#######################################
# Build recipes
#######################################

all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

# List of C objects
OBJECTS		= $(addprefix $(BUILD_DIR)/,$(notdir $(SOURCES_C_FILES:.c=.o)))
vpath %.c $(sort $(dir $(SOURCES_C_FILES)))

# List of CPP objects
OBJECTS		+= $(addprefix $(BUILD_DIR)/,$(notdir $(SOURCES_CPP_FILES:.cpp=.opp)))
vpath %.cpp $(sort $(dir $(SOURCES_CPP_FILES)))

# List of Assembly objects
OBJECTS		+= $(addprefix $(BUILD_DIR)/,$(notdir $(SOURCES_AXX_FILES:.s=.o)))
vpath %.s $(sort $(dir $(SOURCES_AXX_FILES)))

$(BUILD_DIR)/%.opp: %.cpp Makefile | $(BUILD_DIR)
	$(CCPP) -c $(FLAGS_CXX) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.cpp=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	$(CCPP) -c $(FLAGS_CXX) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(FLAGS_CXX) $< -o $@

$(BUILD_DIR)/%.o: %.asm Makefile | $(BUILD_DIR)
	$(AS) -c $(FLAGS_CXX) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(LD) $(OBJECTS) $(LDFLAGS) -o $@
	$(SIZE) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@

$(BUILD_DIR):
	mkdir $@

#######################################
# clean up
#######################################
clear:
	cmd //C del //Q //F *.o *.opp
	cmd //C rmdir //Q //S $(BUILD_DIR)

#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)
