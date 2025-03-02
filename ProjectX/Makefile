# Makefile

# Compiler and flags
CC = gcc
CFLAGS = -Wall -g -O0 -I./header

# Directories
SRC_DIR = src
OBJ_DIR = obj
HEADER_DIR = header

# Main project source files and corresponding object files
SRC_FILES = $(SRC_DIR)/main.c $(SRC_DIR)/parser.c $(SRC_DIR)/spreadsheet.c
OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Unit test source file and object file
TEST_SRC = $(SRC_DIR)/parse_command_unit_test.c
TEST_OBJ = $(OBJ_DIR)/parse_command_unit_test.o

# Output executable names
MAIN_EXE = sheet
TEST_EXE = parse_command_unit_test

# Default target builds the main executable
all: $(MAIN_EXE)

# Link the main executable
$(MAIN_EXE): $(OBJ_FILES)
	$(CC) -o $(MAIN_EXE) $(OBJ_FILES) -lm

# Compile .c files to .o files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# 'tests' target builds the test executable and then runs it.
tests: $(TEST_EXE)
	./$(TEST_EXE)

# Link the test executable with the required object files
$(TEST_EXE): $(TEST_OBJ) $(OBJ_DIR)/parser.o $(OBJ_DIR)/spreadsheet.o
	$(CC) -o $(TEST_EXE) $(TEST_OBJ) $(OBJ_DIR)/parser.o $(OBJ_DIR)/spreadsheet.o -lm

# Clean up object files and executables
clean:
	rm -rf $(OBJ_DIR) $(MAIN_EXE) $(TEST_EXE)

.PHONY: all tests clean
