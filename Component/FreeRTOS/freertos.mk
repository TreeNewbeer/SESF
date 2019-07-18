RTOS_SRCS += croutine.c
RTOS_SRCS += event_groups.c
RTOS_SRCS += list.c
RTOS_SRCS += queue.c
RTOS_SRCS += tasks.c
RTOS_SRCS += timers.c
RTOS_SRCS += $(RTOS_DIR)/portable/MemMang/heap_4.c
RTOS_SRCS += $(RTOS_DIR)/portable/GCC/ARM_CM4F/port.c

VPATH += :$(RTOS_DIR)

RTOS_INCS += "-I$(RTOS_DIR)/include"
RTOS_INCS += "-I$(RTOS_DIR)/portable/GCC/ARM_CM4F"