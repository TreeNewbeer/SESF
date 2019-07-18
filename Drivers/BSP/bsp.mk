BSP_SRCS += $(BSP_DIR)/STM32F429I-Discovery/stm32f429i_discovery.c
BSP_SRCS += $(BSP_DIR)/STM32F429I-Discovery/stm32f429i_discovery_lcd.c
BSP_SRCS += $(BSP_DIR)/STM32F429I-Discovery/stm32f429i_discovery_sdram.c
BSP_SRCS += $(BSP_DIR)/Components/ili9341/ili9341.c

VPATH += :$(BSP_DIR)

BSP_INCS += "-I$(BSP_DIR)/STM32F429I-Discovery"
BSP_INCS += "-I$(BSP_DIR)/Components/ili9341"
BSP_INCS += "-I$(BSP_DIR)/Utilities/Fonts"