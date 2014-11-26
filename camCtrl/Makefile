PROJECT_NAME	= CameraControl_AVT

PROJECT_DIR 		= .
VIMBASDK_DIR		= ./AVT_sdk/Vimba_1_2
MAKE_INCLUDE_DIR 	= ./makeInclude

include $(MAKE_INCLUDE_DIR)/Common.mk

CONFIG_DIR          = x86_$(WORDSIZE)bit
BIN_FILE            = $(PROJECT_NAME)
BIN_DIR             = bin/$(CONFIG_DIR)
OBJ_DIR             = obj/$(CONFIG_DIR)
BIN_PATH            = $(BIN_DIR)/$(BIN_FILE)
INCLUDE_DIR 	    = $(PROJECT_DIR)/include

all: $(BIN_PATH)

include $(MAKE_INCLUDE_DIR)/VimbaCPP.mk
include $(MAKE_INCLUDE_DIR)/CopyData.mk

SOURCE_DIR	 = $(PROJECT_DIR)/src
INCLUDE_DIRS	 = -I$(INCLUDE_DIR)

CFLAGS		+= $(shell pkg-config opencv --cflags)

LIBS 		+= -ltiff
LIBS		+= $(VIMBACPP_LIBS) 

DEFINES 	= 

BUILD_WITH_GUI = 1

ifeq ($(BUILD_WITH_GUI),1)
DEFINES		+= -DCOMPILE_WITH_GUI=1
LIBS 		+= -lopencv_core  -lopencv_highgui
endif

CFLAGS		= $(COMMON_CFLAGS) \
		  $(VIMBACPP_CFLAGS)

OBJ_FILES	= $(OBJ_DIR)/main.o \
		  $(OBJ_DIR)/camCtrlVmbAPI.o \
		  $(OBJ_DIR)/minIni.o \
		  $(OBJ_DIR)/iniReader.o\
		  $(OBJ_DIR)/experiments.o\
		  $(OBJ_DIR)/fileIO.o\
		  $(OBJ_DIR)/commonImage.o\
		  $(OBJ_DIR)/GT1290Camera.o



DEPENDENCIES	= VimbaCPP CopySettings


$(OBJ_DIR)/%.o: $(SOURCE_DIR)/%.cpp $(OBJ_DIR) 
	$(CXX) -c $(INCLUDE_DIRS) $(DEFINES) $(CFLAGS) -m$(WORDSIZE) -o $@ $<

$(BIN_PATH): $(DEPENDENCIES) $(OBJ_FILES) $(BIN_DIR)
	$(CXX) -m$(WORDSIZE) -o $(BIN_PATH) $(OBJ_FILES) $(LIBS) -Wl,-rpath,'$$ORIGIN'

clean:
	$(RM) bin -r -f
	$(RM) obj -r -f
	$(RM) doc/html -r -f

$(OBJ_DIR):
	$(MKDIR) -p $(OBJ_DIR)

$(BIN_DIR):
	$(MKDIR) -p $(BIN_DIR)

$(DOC_DIR):
	$(MKDIR) -p $(DOC_DIR)

edit: 
	emacs Makefile src/*.cpp include/*.h data/setup.ini &

wc:
	wc -l src/*.cpp include/*.h data/setup.ini

mydoc: 	
	doxygen doc/config
	echo "Documentation created under ./doc/html"

vv:
	AVT_sdk/Vimba_1_2/VimbaCPP/Examples/Bin/x86_64bit/VimbaViewer &
