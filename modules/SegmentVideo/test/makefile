# STM32F4xx bsp uart test
REPO_ROOT = ../../..

ifeq ($(strip $(board)),)
ifneq ('$(MAKECMDGOALS)','clean')
$(info no board specified, setting to clock)
board = clock
endif
endif

#Unused:
#BlinkerPanel.cpp
#MidiClockDisplay.cpp
C_SOURCES =  \
main.cpp \
taskTest.cpp \
taskConsole.cpp \
taskLog.cpp \
os.c

SOURCE_DIRS = . \

# TODO: Not all these should be included maybe
# TODO: FreeRTOSConfig.h location?
C_INCLUDES = \
$(REPO_ROOT)/bsp \
$(REPO_ROOT)/modules/SegmentVideo \
$(REPO_ROOT)/modules/logging \
$(REPO_ROOT)/FreeRTOS \
$(REPO_ROOT)/FreeRTOS/Source/include \
$(REPO_ROOT)/FreeRTOS/Source/CMSIS_RTOS \
$(REPO_ROOT)/FreeRTOS/Source/portable/GCC/ARM_CM4F

STATIC_LIBS = \
$(REPO_ROOT)/FreeRTOS/build/FreeRTOS.a \
$(REPO_ROOT)/modules/SegmentVideo/build/SegmentVideo.a \
$(REPO_ROOT)/modules/logging/build/logging.a \
$(REPO_ROOT)/bsp/build/bsp_$(board).a

# objects
OBJECTS = $(patsubst %c,%o,$(patsubst %cpp,%o,$(C_SOURCES)))

# rules
default: all

include $(REPO_ROOT)/tools/build/catalog.mk

include $(REPO_ROOT)/tools/build/common.mk

# linker
$(BUILD_DIR)/output.elf: build_dirs $(addprefix $(BUILD_DIR)/obj/, $(OBJECTS))
	@echo "#### Linking into elf"
	$(PP) -o $(TARGET_EXEC) $(LDFLAGS) -std=c++11 $(addprefix $(BUILD_DIR)/obj/, $(OBJECTS)) $(STATIC_LIBS)
	$(SZ) $@
	@echo ""
	@echo "Built for $(board) board"
	@echo ""
	$(OBJD) -S --disassemble $(TARGET_EXEC) > $(TARGET_EXEC).dump
	@echo

$(BUILD_DIR)/output.hex: build_dirs $(TARGET_EXEC)
	@echo "#### Elf to hex"
	$(HEX) $< $@
	@echo

$(BUILD_DIR)/output.bin: build_dirs $(TARGET_EXEC)
	@echo "#### Elf to bin"
	$(BIN) $< $@
	@echo

.PHONY: $(STATIC_LIBS)

all: $(STATIC_LIBS) $(TARGET_EXEC)

clean:: clean_all

openocd:
	cygstart cmd /k $(shell cygpath -w $(PATH_OPENOCD)/launch_openocd.bat)

gdb:
	@echo ""
	@echo "gdb needs special ctrl-c treatment."
	@echo "Launch cmd.exe from start and enter these commands:"
	@echo ""
	@echo "E:"
	@echo "cd $(shell cygpath -w $(shell pwd))"
	@echo "$(shell cygpath -w $(GDB)) -x gdbinit.txt build/output.elf"

info:
	@echo "***** ***** ***** DEBUG ***** ***** *****"
	@echo $(addprefix $(BUILD_DIR)/obj/, $(OBJECTS))
	@echo $(C_INCLUDES) 
