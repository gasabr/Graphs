##########################################
#           Editable options             #
##########################################

# Compiler options
CC=g++
CFLAGS=-c -Wall -std=c++11
LDFLAGS=
EXECUTABLE_NAME=test

# Folders
SRC=src
BIN=build
OBJ=$(BIN)/obj

# Files
SOURCE_FILES=\
    vertex.cpp \
    graph.cpp \
    test.cpp

##########################################
#    Don't touch anything below this     #
##########################################
OBJECT_FILES=$(addprefix $(OBJ)/, $(SOURCE_FILES:.cpp=.o))

build: create_directories create_executable
	@echo "Build successful!"

create_executable: create_objects
	@$(CC) $(LDFLAGS) $(OBJECT_FILES) -o $(EXECUTABLE_NAME)
	@echo "Created executable."

create_objects: $(SOURCE_FILES)
	@echo "Created objects."

create_directories: 
	@mkdir -p $(OBJ)

%.cpp:
	@echo "Compiling "$@
	@$(CC) $(CFLAGS) $(LDFLAGS) -c $(SRC)/$@ -o $(OBJ)/$(patsubst %.cpp,%.o,$@)

clean:
	@rm -r -f $(BIN)