# _template module
SYNTH_COMMON_ROOT := ../..
REPO_ROOT = ../$(SYNTH_COMMON_ROOT)

C_SOURCES = \
src/template.cpp \



SOURCE_DIRS = ./ \
src

C_INCLUDES = \
$(REPO_ROOT)/bsp \

#PROJECT_OBJECTS
OBJECTS = $(patsubst %s,%o,$(patsubst %c,%o,$(patsubst %cpp,%o,$(C_SOURCES))))

OUTPUT_LIB_NAME = template.a

default: info all

include $(SYNTH_COMMON_ROOT)/tools/build/common.mk

all: $(BUILD_DIR)/$(OUTPUT_LIB_NAME)

info:
	@echo "***** ***** ***** DEBUG ***** ***** *****"
	@echo $(addprefix $(BUILD_DIR)/obj/, $(OBJECTS))
	@echo $(C_INCLUDES)
	@echo $(OBJECTS)
