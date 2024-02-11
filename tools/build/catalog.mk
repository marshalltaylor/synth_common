#######################################
# Paths
#######################################
# Inputs:
# REPO_ROOT
# SYNTH_COMMON_ROOT

PATH_OPENOCD:=$(SYNTH_COMMON_ROOT)/tools/debugger


$(REPO_ROOT)/bsp/build/bsp.a:
	@echo "######## Building $(REPO_ROOT)/bsp"
	$(MAKE) -C $(REPO_ROOT)/bsp

$(SYNTH_COMMON_ROOT)/FreeRTOS/build/FreeRTOS.a:
	@echo "######## Building $(SYNTH_COMMON_ROOT)/FreeRTOS"
	$(MAKE) -C $(SYNTH_COMMON_ROOT)/FreeRTOS

$(SYNTH_COMMON_ROOT)/modules/logging/build/logging.a:
	@echo "######## Building $(SYNTH_COMMON_ROOT)/modules/logging"
	$(MAKE) -C $(SYNTH_COMMON_ROOT)/modules/logging

$(SYNTH_COMMON_ROOT)/modules/midi47fx/build/midi47fx.a:
	@echo "######## Building $(SYNTH_COMMON_ROOT)/modules/midi47fx"
	$(MAKE) -C $(SYNTH_COMMON_ROOT)/modules/midi47fx

$(SYNTH_COMMON_ROOT)/modules/CRTVideo/build/CRTVideo.a:
	@echo "######## Building $(SYNTH_COMMON_ROOT)/modules/CRTVideo"
	$(MAKE) -C $(SYNTH_COMMON_ROOT)/modules/CRTVideo

$(SYNTH_COMMON_ROOT)/modules/uCModules/build/uCModules.a:
	@echo "######## Building $(SYNTH_COMMON_ROOT)/modules/uCModules"
	$(MAKE) -C $(SYNTH_COMMON_ROOT)/modules/uCModules

$(SYNTH_COMMON_ROOT)/modules/midiUtils/build/midiUtils.a:
	@echo "######## Building $(SYNTH_COMMON_ROOT)/modules/midiUtils"
	$(MAKE) -C $(SYNTH_COMMON_ROOT)/modules/midiUtils

$(SYNTH_COMMON_ROOT)/modules/TeensyView/build/TeensyView.a:
	@echo "######## Building $(SYNTH_COMMON_ROOT)/modules/TeensyView"
	$(MAKE) -C $(SYNTH_COMMON_ROOT)/modules/TeensyView

clean_all:
	$(MAKE) -C $(REPO_ROOT)/bsp clean
	$(MAKE) -C $(SYNTH_COMMON_ROOT)/FreeRTOS clean
	$(MAKE) -C $(SYNTH_COMMON_ROOT)/modules/logging clean
	$(MAKE) -C $(SYNTH_COMMON_ROOT)/modules/midi47fx clean
	$(MAKE) -C $(SYNTH_COMMON_ROOT)/modules/midiUtils clean
	$(MAKE) -C $(SYNTH_COMMON_ROOT)/modules/CRTVideo clean
	$(MAKE) -C $(SYNTH_COMMON_ROOT)/modules/TeensyView clean
	$(MAKE) -C $(SYNTH_COMMON_ROOT)/modules/uCModules clean

clean_apps:
	rm -rf $(REPO_ROOT)/apps/sliders_001/build
	rm -rf $(REPO_ROOT)/bsp/test/build
	rm -rf $(SYNTH_COMMON_ROOT)/modules/_template/test/build
	rm -rf $(SYNTH_COMMON_ROOT)/modules/logging/test/build
	rm -rf $(SYNTH_COMMON_ROOT)/modules/midi47fx/test/build
	rm -rf $(SYNTH_COMMON_ROOT)/modules/midiUtils/test/build
	rm -rf $(SYNTH_COMMON_ROOT)/modules/CRTVideo/test/build
	rm -rf $(SYNTH_COMMON_ROOT)/modules/TeensyView/test/build
	rm -rf $(SYNTH_COMMON_ROOT)/modules/uCModules/test/build
