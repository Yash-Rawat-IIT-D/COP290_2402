# To be Done 

# Compiler and flags
CC = gcc
CFLAGS = -Wall -g -O0 -I./header  # Include the header directory for header files

# Directories
SRC_DIR = src
OBJ_DIR = obj
HEADER_DIR = header

# Source files
SRC_FILES = $(SRC_DIR)/main.c \
            $(SRC_DIR)/parser.c \
			$(SRC_DIR)/spreadsheet.c\
			

# Object files (replace .c with .o)
OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Output executable name
OUTPUT = sheet

# Default target: build the program
all: $(OUTPUT)

# Link object files into the final executable
$(OUTPUT): $(OBJ_FILES)
	$(CC) -o $(OUTPUT) $(OBJ_FILES) -lm

# Compile each .c file to a .o object file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up object files and the binary
clean:
	rm -f $(OBJ_FILES) $(OUTPUT)

# Phony targets (not actual files)
.PHONY: all clean

