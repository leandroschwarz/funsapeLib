# ------------------------------------------------
# STM32 ARM Makefile essentials
# ------------------------------------------------

######################################
# Editable area - BEGIN

TARGET = platform
DEBUG = 0
OPT = -Og

CPU = -mcpu=cortex-m0plus
FPU =
FLOAT-ABI =

DEFS_ASM =
DEFS_CXX = -DUSE_HAL_DRIVER -DSTM32L053xx -DGPDSE_IGNORE_STUBS

LDSCRIPT = STM32L053R8Tx_FLASH.ld

# Editable area - END
######################################

#######################################
# Recursive wildcards
#######################################

rwildcard = $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

#######################################
# Paths
#######################################

# Header path
HEADER_H_PATHS = $(dir $(call rwildcard, ,*.h))
HEADER_HPP_PATHS = $(dir $(call rwildcard, ,*.hpp))
HEADER_HXX_PATHS = $(sort $(HEADER_H_PATHS) $(HEADER_HPP_PATHS))
HEADER_ASM_PATHS = $(sort $(dir $(call rwildcard, ,*.S)))

# Firmware library path
PERIFLIB_PATHS =

# Build directory path
BUILD_DIR = build

######################################
# Source files
######################################

# Assembly source files
SOURCES_ASM_FILES = $(call rwildcard, ,*.asm)
SOURCES_S_FILES   = $(call rwildcard, ,*.s)
SOURCES_AXX_FILES = $(sort $(SOURCES_ASM_FILES) $(SOURCES_S_FILES))

# C source files
SOURCES_C_FILES   = $(call rwildcard, ,*.c)
SOURCES_CPP_FILES = $(call rwildcard, ,*.cpp)
SOURCES_CXX_FILES = $(sort $(SOURCES_C_FILES) $(SOURCES_CPP_FILES))

# Firmware library files
PERIFLIB_SOURCES =

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

# Device flag
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# Include paths
INCLUDES_ASM = $(addprefix -I, $(HEADER_ASM_PATHS))
INCLUDES_HXX = $(addprefix -I, $(HEADER_HXX_PATHS))

# Assembler flags
FLAGS_ASM = $(MCU) $(DEFS_ASM) $(INCLUDES_ASM) $(OPT) -Wall -fdata-sections -ffunction-sections

# Compiler flags
FLAGS_CXX = $(MCU) $(DEFS_CXX) $(INCLUDES_HXX) $(OPT) -Wall -fdata-sections -ffunction-sections -Wno-write-strings
ifeq ($(DEBUG), 1)
	FLAGS_CXX += -g -gdwarf-2
endif

# Generate dependency information
FLAGS_CXX += -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)"

#######################################
# Linker flags
#######################################

# Libraries
LIBS = -lc -lm -lnosys
LIBDIR =

FLAGS_LINKER = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections -specs=nosys.specs

#######################################
# Build recipes
#######################################

.PHONY: compile program cleanCompile cleanCompileProgram clean compileProgram compileCleanProgram cleanCompileCleanProgram

#######################################
# Compile
#######################################

compile: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

# List of C objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(SOURCES_C_FILES:.c=.o)))
vpath %.c $(sort $(dir $(SOURCES_C_FILES)))

# List of CPP objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(SOURCES_CPP_FILES:.cpp=.opp)))
vpath %.cpp $(sort $(dir $(SOURCES_CPP_FILES)))

# List of Assembly objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(SOURCES_AXX_FILES:.s=.o)))
vpath %.s $(sort $(dir $(SOURCES_AXX_FILES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	$(CC) -c $(FLAGS_CXX) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.opp: %.cpp Makefile | $(BUILD_DIR)
	$(CCPP) -c $(FLAGS_CXX) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.cpp=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(FLAGS_CXX) $< -o $@

$(BUILD_DIR)/%.o: %.asm Makefile | $(BUILD_DIR)
	$(AS) -c $(FLAGS_CXX) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(FLAGS_LINKER) -o $@
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

cleanCompile:	clean compile

#######################################
# Program - Program target
#######################################

program:
	ST-LINK_CLI -c SWD UR -P $(BUILD_DIR)/$(TARGET).hex -V -HardRst


compileProgram: compile program

cleanCompileProgram: clean compile program

compileCleanProgra: compile clean program

cleanCompileCleanProgram: clean compile clean program

#######################################
# dependencies
#######################################
# -include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)
