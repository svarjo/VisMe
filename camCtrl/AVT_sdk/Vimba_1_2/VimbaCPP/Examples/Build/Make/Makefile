EXAMPLES_DIR            = ../..
MAKE_INCLUDE_DIR        = $(CURDIR)

include $(MAKE_INCLUDE_DIR)/Common.mk

EXAMPLES				= AsynchronousGrab \
						  BandwidthHelper \
						  CameraFactory \
						  ListCameras \
						  ListFeatures \
						  LoadSaveSettings \
						  LookUpTable \
						  ShadingData \
						  SynchronousGrab \
						  UserSet \
						  VimbaViewer

make_%: $(EXAMPLES_DIR)/%/Build/Make/Makefile
	$(MAKE) -C $(EXAMPLES_DIR)/$*/Build/Make
	
make_%: $(EXAMPLES_DIR)/%/Qt/Build/Make/Makefile
	$(MAKE) -C $(EXAMPLES_DIR)/$*/Qt/Build/Make

clean_%: $(EXAMPLES_DIR)/%/Build/Make/Makefile
	$(MAKE) -C $(EXAMPLES_DIR)/$*/Build/Make clean

clean_%: $(EXAMPLES_DIR)/%/Qt/Build/Make/Makefile
	$(MAKE) -C $(EXAMPLES_DIR)/$*/Qt/Build/Make clean

all: $(foreach example,$(EXAMPLES),make_$(example))

clean: $(foreach example,$(EXAMPLES),clean_$(example))
