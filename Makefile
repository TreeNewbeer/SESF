TARGET = SESF
BUILD_DIR = build


######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -Og

#######################################
# Load  Compontent
#######################################
PORT_DIR = Component/port

include $(PORT_DIR)/freertos/freertos.mk
include $(PORT_DIR)/lvgl/lvgl.mk

PER_DIR = Component/Peripheral
include $(PER_DIR)/peripheral.mk


#######################################
# Load  Drivers
#######################################
CMSIS_DIR = Drivers/CMSIS
include $(CMSIS_DIR)/cmsis.mk

STM_HAL_DIR = Drivers/STM32F4xx_HAL_Driver
include $(STM_HAL_DIR)/stm_hal_driver.mk

BSP_DIR = Drivers/BSP
include $(BSP_DIR)/bsp.mk


#######################################
# Load  App
#######################################
APP_DIR = App
include $(APP_DIR)/app.mk

######################################
# source
######################################
# C sources
C_SOURCES += $(CMSIS_SRCS)
C_SOURCES += $(STM_HAL_SRCS)
C_SOURCES += $(PER_SRCS)
C_SOURCES += $(BSP_SRCS)
C_SOURCES += $(RTOS_SRCS)
C_SOURCES += $(LVGL_SRCS)
C_SOURCES += $(APP_SRCS)

# ASM sources
ASM_SOURCES =  \
startup_stm32f429xx.s


#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S


#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m4

# fpu
FPU = -mfpu=fpv4-sp-d16

# float-abi
FLOAT-ABI = -mfloat-abi=hard

# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS =  \
-DUSE_HAL_DRIVER \
-DSTM32F429xx \
-DARM_MATH_CM4 \
-D__FPU_PRESENT \
-DLV_CONF_INCLUDE_SIMPLE


# AS includes
AS_INCLUDES = 

# C includes
C_INCLUDES += $(CMSIS_INCS)
C_INCLUDES += $(STM_HAL_INCS)
C_INCLUDES += $(PER_INCS)
C_INCLUDES += $(BSP_INCS)
C_INCLUDES += $(RTOS_INCS)
C_INCLUDES += $(LVGL_INCS)
C_INCLUDES += $(PER_INCS)

# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections


ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"


#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = STM32F429ZITx_FLASH.ld

# libraries
LIBS = -lc -lm -lnosys 
LIBDIR = -LDrivers/CMSIS/Lib/GCC
LDFLAGS = $(MCU) -specs=rdimon.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

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
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)
  
#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***
