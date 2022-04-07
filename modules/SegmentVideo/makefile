# SegmentVideo Module
SYNTH_COMMON_ROOT := ../..
REPO_ROOT = ../$(SYNTH_COMMON_ROOT)

$(info )
$(info Warning:)
$(info The SegmentVideo module is not generic and has been moved to the using app.)
$(info Whatever you're building needs to take a copy of the source and use it directly.)
$(info )
$(info TODO: Make a more basic segment driver module)

C_SOURCES = \
src/SegmentVideo.cpp \
src/MidiClockDisplay.cpp

SOURCE_DIRS = src

C_INCLUDES = . \
src \
$(REPO_ROOT)/bsp

#PROJECT_OBJECTS
OBJECTS = $(patsubst %s,%o,$(patsubst %c,%o,$(patsubst %cpp,%o,$(C_SOURCES))))

OUTPUT_LIB_NAME = SegmentVideo.a

default: info all

include $(SYNTH_COMMON_ROOT)/tools/build/common.mk

all: $(BUILD_DIR)/$(OUTPUT_LIB_NAME)

info:
	@echo "***** ***** ***** DEBUG ***** ***** *****"
	@echo $(addprefix $(BUILD_DIR)/obj/, $(OBJECTS))
	@echo $(C_INCLUDES)
	@echo $(OBJECTS)
