PROJECT_NAME	= CameraControl_AVT_GT1290

PROJECT_DIR 		= .
VIMBASDK_DIR		= ./AVT_sdk/Vimba_1_2
MAKE_INCLUDE_DIR 	= ./makeInclude

include $(MAKE_INCLUDE_DIR)/Common.mk

CONFIG_DIR          = x86_$(WORDSIZE)bit
BIN_FILE            = $(PROJECT_NAME)
BIN_DIR             = bin/$(CONFIG_DIR)
OBJ_DIR             = obj/$(CONFIG_DIR)
BIN_PATH            = $(BIN_DIR)/$(BIN_FILE)

all: $(BIN_PATH)

include $(MAKE_INCLUDE_DIR)/VimbaCPP.mk

SOURCE_DIR	 = $(PROJECT_DIR)/src
INCLUDE_DIRS	 = -I$(PROJECT_DIR)/include 

#CFLAGS		+= $(shell pkg-config opencv --cflags)
#LIBS 		+= $(shell pkg-config opencv --libs)

LIBS		= $(VIMBACPP_LIBS)
DEFINES		=
CFLAGS		= $(COMMON_CFLAGS) \
		  $(VIMBACPP_CFLAGS)

OBJ_FILES	= $(OBJ_DIR)/main.o \
		  $(OBJ_DIR)/camCtrlVmbAPI.o \
		  $(OBJ_DIR)/minIni.o \
		  $(OBJ_DIR)/iniReader.o

DEPENDENCIES	= VimbaCPP

SOURCES	= $(SRCDIR)/main.cpp $(SRCDIR)/camCtrl.cpp $(SRCDIR)/minIni.cpp $(SRCDIR)/iniReader.cpp

OBJECTS=$(OBJ_DIR)/$(SOURCES:.cpp.c=.o) 

$(OBJ_DIR)/%.o: $(SOURCE_DIR)/%.cpp $(OBJ_DIR) 
	$(CXX) -c $(INCLUDE_DIRS) $(DEFINES) $(CFLAGS) -m$(WORDSIZE) -o $@ $<

$(BIN_PATH): $(DEPENDENCIES) $(OBJ_FILES) $(BIN_DIR)
	$(CXX) -m$(WORDSIZE) -o $(BIN_PATH) $(OBJ_FILES) $(LIBS) -Wl,-rpath,'$$ORIGIN'

clean:
	$(RM) bin -r -f
	$(RM) obj -r -f

$(OBJ_DIR):
	$(MKDIR) -p $(OBJ_DIR)

$(BIN_DIR):
	$(MKDIR) -p $(BIN_DIR)
