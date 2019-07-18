APP_SRCS += $(APP_DIR)/Src/main.c
APP_SRCS += $(APP_DIR)/Src/stm32f4xx_it.c
APP_SRCS += $(APP_DIR)/Src/system_stm32f4xx.c
APP_SRCS += $(APP_DIR)/Src/stm32f4xx_hal_msp.c
APP_SRCS += $(APP_DIR)/Src/stm32f4xx_hal_timebase_tim.c

VPATH += :$(APP_DIR)

PER_INCS += "-I$(APP_DIR)/Inc"