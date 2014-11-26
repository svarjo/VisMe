PROJECT_NAME	= viewHDRtif

PROJECT_DIR 		= .

BIN_FILE            = $(PROJECT_NAME)
BIN_DIR             = bin
OBJ_DIR             = obj
BIN_PATH            = $(BIN_DIR)/$(BIN_FILE)
INCLUDE_DIR 	    = .

all: $(BIN_PATH)

SRC_DIR		 = $(PROJECT_DIR)/.
INCLUDE_DIRS	 = -I$(INCLUDE_DIR)

SOURCE_DIR 	=$(SRC_DIR)

CFLAGS		+= $(shell pkg-config opencv --cflags)
LIBS 		+= -lopencv_core  -lopencv_highgui

LIBS 		+= -ltiff
LIBS		+= $(VIMBACPP_LIBS) 

DEFINES		=
CFLAGS		= 

OBJ_FILES	= $(OBJ_DIR)/viewHDR.o \
		  $(OBJ_DIR)/commonImage.o

DEPENDENCIES	= 

SOURCES	= 	$(SRCDIR)/viewHDR.cpp \
		$(SRCDIR)/commonImage.cpp

MKDIR = mkdir
RM = rm
WORDSIZE = 64


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

$(DOC_DIR):
	$(MKDIR) -p $(DOC_DIR)

edit: 
	emacs Makefile .cpp *.h &

wc:
	wc -l *.cpp *.h 
