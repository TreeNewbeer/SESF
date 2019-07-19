LV_PORT_DIR = Component/port/lvgl
LVGL_DIR = Component/lvgl


LVGL_SRCS += $(wildcard ${LVGL_DIR}/src/lv_core/*.c)
LVGL_SRCS += $(wildcard ${LVGL_DIR}/src/lv_hal/*.c)
LVGL_SRCS += $(wildcard ${LVGL_DIR}/src/lv_objx/*.c)
LVGL_SRCS += $(wildcard ${LVGL_DIR}/src/lv_font/*.c)
LVGL_SRCS += $(wildcard ${LVGL_DIR}/src/lv_misc/*.c)
LVGL_SRCS += $(wildcard ${LVGL_DIR}/src/lv_themes/*.c)
LVGL_SRCS += $(wildcard ${LVGL_DIR}/src/lv_draw/*.c)


DEPPATH += --dep-path $(LVGL_DIR)
DEPPATH += --dep-path $(LVGL_DIR)/src
DEPPATH += --dep-path $(LVGL_DIR)/src/lv_core
DEPPATH += --dep-path $(LVGL_DIR)/src/lv_hal
DEPPATH += --dep-path $(LVGL_DIR)/src/lv_objx
DEPPATH += --dep-path $(LVGL_DIR)/src/lv_font
DEPPATH += --dep-path $(LVGL_DIR)/src/lv_misc
DEPPATH += --dep-path $(LVGL_DIR)/src/lv_themes
DEPPATH += --dep-path $(LVGL_DIR)/src/lv_draw


VPATH += :$(LVGL_DIR)/src/lv_core:$(LVGL_DIR)/src/lv_hal \
		 :$(LVGL_DIR)/src/lv_objx:$(LVGL_DIR)/src/lv_font \
		 :$(LVGL_DIR)/src/lv_misc:$(LVGL_DIR)/src/lv_themes \
		 :$(LVGL_DIR)/src/lv_draw:$(LVGL_DIR)/src


LVGL_INCS = "-I$(LV_PORT_DIR)" \
			"-I$(LVGL_DIR)" \
			"-I$(LVGL_DIR)/src/" \
			"-I$(LVGL_DIR)/src/lv_core" \
			"-I$(LVGL_DIR)/src/lv_hal" \
			"-I$(LVGL_DIR)/src/lv_objx" \
			"-I$(LVGL_DIR)/src/lv_font" \
			"-I$(LVGL_DIR)/src/lv_misc" \
			"-I$(LVGL_DIR)/src/lv_themes" \
			"-I$(LVGL_DIR)/src/lv_draw"
