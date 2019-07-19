RTOS_PORT_DIR = $(PORT_DIR)/freertos
RTOS_DIR = Component/FreeRTOS

RTOS_SRCS = $(RTOS_DIR)/croutine.c \
			$(RTOS_DIR)/event_groups.c \
			$(RTOS_DIR)/list.c \
			$(RTOS_DIR)/queue.c \
			$(RTOS_DIR)/tasks.c \
			$(RTOS_DIR)/timers.c \
			$(RTOS_DIR)/portable/MemMang/heap_4.c \
			$(RTOS_DIR)/portable/GCC/ARM_CM4F/port.c

VPATH += :$(RTOS_DIR)


RTOS_INCS = "-I$(RTOS_PORT_DIR)" \
			"-I$(RTOS_DIR)/include" \
			"-I$(RTOS_DIR)/portable/GCC/ARM_CM4F"
