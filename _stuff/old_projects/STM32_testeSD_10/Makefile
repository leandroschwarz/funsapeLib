# ------------------------------------------------
# STM32 ARM Makefile essentials
# ------------------------------------------------

######################################
# Target name
######################################

TARGET = testeSD

######################################
# Building variables
######################################

DEBUG = 0
OPT = -Og

#######################################
# Recursive wildcards
#######################################

rwildcard = $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))


#######################################
# Paths
#######################################

# Source files path
DIRECTORIES = \
Drivers/BSP/Adafruit_Shield											\
Drivers/BSP/STM32L0xx_Nucleo										\
Drivers/CMSIS/Device/ST/STM32L0xx/Include							\
Drivers/CMSIS/Include												\
Drivers/STM32L0xx_HAL_Driver/Inc									\
FatFs																\
FatFs/drivers														\
Inc		\


# Firmware library path
PERIFLIB_PATH =

# Build directory path
BUILD_DIR = build

######################################
# Source files
######################################

# Assembly Source files (ACTIVE)
ASM_SOURCES = \
startup_stm32l053xx.s	\

# Assembly Source files (INACTIVE)
# none

# C Source files (ACTIVE)
C_SOURCES = \
Drivers/BSP/STM32L0xx_Nucleo/STM32L0xx_Nucleo.c									\
Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal.c								\
Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_cortex.c							\
Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_gpio.c							\
Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_rcc.c							\
Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_spi.c							\
FatFs/diskio.c										\
FatFs/drivers/stm32_adafruit_sd.c							\
FatFs/drivers/sd_diskio.c							\
FatFs/ff.c											\
FatFs/ff_gen_drv.c									\
Src/main.c				\
Src/stm32l0xx_it.c		\
Src/system_stm32l0xx.c	\

#Drivers/BSP/Adafruit_Shield/stm32_adafruit_sd.c									\
#Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_rcc_ex.c				\
#Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_pwr.c				\
#Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_adc.c				\
#Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_adc_ex.c				\
#Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_dac.c				\
#Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_dac_ex.c				\
#Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_dma.c				\
#Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_flash.c				\
#Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_flash_ex.c			\
#Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_flash_ramfunc.c		\
#Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_i2c.c				\
#Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_i2c_ex.c				\
#Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_pwr_ex.c				\
#Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_rtc.c				\
#Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_rtc_ex.c				\
#Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_tim.c				\
#Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_tim_ex.c				\
#Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_uart.c				\
#Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_uart_ex.c			\
#FatFs/option/syscall.c					\
#Src/adc.c															\
#Src/dac.c															\
#Src/dma.c															\
#Src/fatfs.c															\
#Src/gpio.c															\
#Src/i2c.c															\
#Src/rtc.c															\
#Src/spi.c															\
#Src/stm32l0xx_hal_msp.c												\
#Src/stm32l0xx_it.c													\
#Src/system_stm32l0xx.c												\
#Src/tim.c															\
#Src/usart.c															\
#Src/user_diskio.c													\

# C Source files (INACTIVE)
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_comp.c				\
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_comp_ex.c			\
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_crc.c				\
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_crc_ex.c				\
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_cryp.c				\
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_cryp_ex.c			\
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_dac_ex.c				\
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_firewall.c			\
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_i2s.c				\
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_irda.c				\
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_iwdg.c				\
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_lcd.c				\
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_lptim.c				\
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_pcd.c				\
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_pcd_ex.c				\
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_rng.c				\
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_smartcard.c			\
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_smartcard_ex.c		\
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_smbus.c				\
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_tsc.c				\
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_usart.c				\
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_wwdg.c				\
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_ll_adc.c					\
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_ll_comp.c				\
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_ll_crc.c					\
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_ll_crs.c					\
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_ll_dac.c					\
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_ll_dma.c					\
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_ll_exti.c				\
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_ll_gpio.c				\
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_ll_i2c.c					\
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_ll_lptim.c				\
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_ll_lpuart.c				\
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_ll_pwr.c					\
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_ll_rcc.c					\
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_ll_rng.c					\
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_ll_rtc.c					\
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_ll_spi.c					\
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_ll_tim.c					\
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_ll_usart.c				\
# Drivers/STM32L0xx_HAL_Driver/Src/stm32l0xx_ll_utils.c				\
# FatFs/option/cc932.c					\
# FatFs/option/cc936.c					\
# FatFs/option/cc949.c					\
# FatFs/option/cc950.c					\
# FatFs/option/ccsbcs.c					\
# FatFs/option/unicode.c					\

# CPP Source files (ACTIVE)
CPP_SOURCES = \


# CPP Source files (INACTIVE)
# none

# Firmware library files
PERIFLIB_SOURCES = \

#######################################
# Executable paths
#######################################

# Constants
BINPATH =
PREFIX = arm-none-eabi-

ifeq ($(BINPATH), )
	AR   =
	AS   =
	BIN  =
	CC   =
	CCPP =
	CP   =
	HEX   =
	SZ   =
else
	AR   = $(BINPATH)/
	AS   = $(BINPATH)/
	BIN  = $(BINPATH)/
	CC   = $(BINPATH)/
	CCPP = $(BINPATH)/
	CP   = $(BINPATH)/
	HEX  = $(BINPATH)/
	SZ   = $(BINPATH)/
endif
AR   += $(PREFIX)ar
AS   += $(PREFIX)gcc -x assembler-with-cpp
BIN  += $(PREFIX)objcopy -O binary -S
CC   += $(PREFIX)gcc
CCPP += $(PREFIX)g++
CP   += $(PREFIX)objcopy
HEX  += $(PREFIX)objcopy -O ihex
SZ   += $(PREFIX)size

#######################################
# Compiler flags
#######################################

# cpu
CPU = -mcpu=cortex-m0plus

# fpu		NONE for Cortex-M0/M0+/M3
FPU =

# float-abi
FLOAT-ABI =

# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# Assembly defines
AS_DEFS =

# C defines
CXX_DEFS =  \
-DUSE_HAL_DRIVER \
-DSTM32L053xx

# Assembly directories
AS_INCLUDES =

# C directories
CXX_INCLUDES = $(addprefix -I, $(DIRECTORIES))

# GCC compile Assembly flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

# GCC compile C flags
CXXFLAGS = $(MCU) $(CXX_DEFS) $(CXX_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections -Wno-write-strings
ifeq ($(DEBUG), 1)
	CXXFLAGS += -g -gdwarf-2
endif

# Generate dependency information
CXXFLAGS += -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)"

#######################################
# Linker flags
#######################################

# Link script
LDSCRIPT = STM32L053R8Tx_FLASH.ld

# Libraries
LIBS = -lc -lm -lnosys
LIBDIR =
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections -specs=nosys.specs

#######################################
# Build recipes
#######################################

.PHONY: all clean program cleanCompile

#######################################
# All - Compile
#######################################

all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

# List of C objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

# List of CPP objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(CPP_SOURCES:.cpp=.opp)))
vpath %.cpp $(sort $(dir $(CPP_SOURCES)))

# List of Assembly objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))


$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	$(CC) -c $(CXXFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.opp: %.cpp Makefile | $(BUILD_DIR)
	$(CCPP) -c $(CXXFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.cpp=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CXXFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@

$(BUILD_DIR):
	mkdir $@

#######################################
# Clean - Clean up
#######################################

clean:
	@del *.o *.opp *.d *.lst /s /q 2> NUL
	@echo Project cleaned!

#######################################
# cleanCompile - Clean up than compile
#######################################

cleanCompile:	clean all

#######################################
# Program - Program target
#######################################

program:
	ST-LINK_CLI -c SWD UR -P $(BUILD_DIR)/$(TARGET).hex -V -HardRst


#######################################
# dependencies
#######################################
# -include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)

# *** EOF ***
