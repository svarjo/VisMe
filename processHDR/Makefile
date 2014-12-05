PROJECT_NAME	= processHDR

CXX = g++
MKDIR = mkdir

PROJECT_DIR 		= .

WORDSIZE = 64
CONFIG_DIR          = x86_$(WORDSIZE)bit
BIN_FILE            = $(PROJECT_NAME)
BIN_DIR             = bin/$(CONFIG_DIR)
OBJ_DIR             = obj/$(CONFIG_DIR)
BIN_PATH            = $(BIN_DIR)/$(BIN_FILE)
INCLUDE_DIR 	    = $(PROJECT_DIR)/include

all: $(BIN_PATH)

SOURCE_DIR	 	= $(PROJECT_DIR)/src
INCLUDE_DIRS	= -I$(INCLUDE_DIR) -I/usr/include -I../../tiff-4.0.3/libtiff

TIFFLIBPATH = /home/samivarj/development/tiff-4.0.3/lib

CFLAGS		+= -Wall -pedantic 
#CFLAGS		+= -Wall -O0 -g -pedantic
#CFLAGS		+= $(shell pkg-config opencv --cflags)

LIBS 		+= -L../../tiff-4.0.3/lib -ltiff -lstdc++

DEFINES 	= 

CFLAGS		+= $(COMMON_CFLAGS) 

OBJ_FILES	= 	$(OBJ_DIR)/main.o \
				$(OBJ_DIR)/fileIO.o\
				$(OBJ_DIR)/imageProcessing.o\
				$(OBJ_DIR)/commonImage.o\
				$(OBJ_DIR)/clahe.o
				
			
$(OBJ_DIR)/%.o: $(SOURCE_DIR)/%.cpp $(OBJ_DIR) 
	$(CXX) -c $(INCLUDE_DIRS) $(DEFINES) $(CFLAGS) -m$(WORDSIZE) -o $@ $<

$(OBJ_DIR)/%.o: $(SOURCE_DIR)/%.c $(OBJ_DIR) 
	$(CXX) -c $(INCLUDE_DIRS) $(DEFINES) $(CFLAGS) -m$(WORDSIZE) -o $@ $<
	
$(BIN_PATH): $(DEPENDENCIES) $(OBJ_FILES) $(BIN_DIR)
	$(CXX) -m$(WORDSIZE) -o $(BIN_PATH) $(OBJ_FILES) $(LIBS) -Wl,-rpath=$(TIFFLIBPATH)
#remind - rpath for locally installed libraries...

$(COBJ): $(SOURCE_DIR)/%.c $(OBJ_DIR)
	$(CXX) -c $(INCLUDE_DIRS) $(DEFINES) $(CFLAGS) -m$(WORDSIZE) -o $@ $<

clean:
	$(RM) bin -r -f
	$(RM) obj -r -f
	$(RM) doc/html -r -f

$(OBJ_DIR):
	$(MKDIR) -p $(OBJ_DIR)

$(BIN_DIR):
	$(MKDIR) -p $(BIN_DIR)

$(DOC_DIR):
	$(MKDIR) $(DOC_DIR)

edit: 
	emacs Makefile src/*.cpp include/*.h data/setup.ini &

wc:
	wc -l src/*.cpp include/*.h data/setup.ini
