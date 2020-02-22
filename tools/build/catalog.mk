#######################################
# Paths
#######################################
# Inputs:
# REPO_ROOT
# board

PATH_OPENOCD:=$(REPO_ROOT)/tools/debugger
#PATH_OPENOCD=$(REPO_ROOT)/tools/debugger


$(REPO_ROOT)/bsp/build/bsp_$(board).a:
	@echo "######## Building $(REPO_ROOT)/bsp_$(board)"
	$(MAKE) -C $(REPO_ROOT)/bsp board=$(board)

$(REPO_ROOT)/FreeRTOS/build/FreeRTOS.a:
	@echo "######## Building $(REPO_ROOT)/FreeRTOS"
	$(MAKE) -C $(REPO_ROOT)/FreeRTOS

$(REPO_ROOT)/modules/logging/build/logging.a:
	@echo "######## Building $(REPO_ROOT)/modules/logging"
	$(MAKE) -C $(REPO_ROOT)/modules/logging

$(REPO_ROOT)/modules/midi47fx/build/midi47fx.a:
	@echo "######## Building $(REPO_ROOT)/modules/midi47fx"
	$(MAKE) -C $(REPO_ROOT)/modules/midi47fx

$(REPO_ROOT)/modules/SegmentVideo/build/SegmentVideo.a:
	@echo "######## Building $(REPO_ROOT)/modules/SegmentVideo"
	$(MAKE) -C $(REPO_ROOT)/modules/SegmentVideo

$(REPO_ROOT)/modules/uCModules/build/uCModules.a:
	@echo "######## Building $(REPO_ROOT)/modules/uCModules"
	$(MAKE) -C $(REPO_ROOT)/modules/uCModules

$(REPO_ROOT)/modules/midiUtils/build/midiUtils.a:
	@echo "######## Building $(REPO_ROOT)/modules/midiUtils"
	$(MAKE) -C $(REPO_ROOT)/modules/midiUtils

$(REPO_ROOT)/modules/TeensyView/build/TeensyView.a:
	@echo "######## Building $(REPO_ROOT)/modules/TeensyView"
	$(MAKE) -C $(REPO_ROOT)/modules/TeensyView

clean_all:
	$(MAKE) -C $(REPO_ROOT)/bsp clean
	$(MAKE) -C $(REPO_ROOT)/FreeRTOS clean
	$(MAKE) -C $(REPO_ROOT)/modules/logging clean
	$(MAKE) -C $(REPO_ROOT)/modules/uCModules clean
	$(MAKE) -C $(REPO_ROOT)/modules/SegmentVideo clean
	$(MAKE) -C $(REPO_ROOT)/modules/midi47fx clean
