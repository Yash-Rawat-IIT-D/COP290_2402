# Makefile

# Compiler and flags
CC = gcc
CFLAGS = -Wall -g -O0 -I./header

# Directories
SRC_DIR = src
OBJ_DIR = obj
HEADER_DIR = header
TARGET_DIR = target/release

# Main project source files and corresponding object files
SRC_FILES = $(SRC_DIR)/main.c $(SRC_DIR)/parser.c $(SRC_DIR)/spreadsheet.c
OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Unit test source file and object file
TEST_SRC = $(SRC_DIR)/parse_command_unit_test.c
TEST_OBJ = $(OBJ_DIR)/parse_command_unit_test.o

# Output executable names
MAIN_EXE = $(TARGET_DIR)/spreadsheet
TEST_EXE = parse_command_unit_test

# Default target builds the main executable
all: $(MAIN_EXE)

# Link the main executable into the target directory
$(MAIN_EXE): $(OBJ_FILES)
	mkdir -p $(TARGET_DIR)
	$(CC) -o $(MAIN_EXE) $(OBJ_FILES) -lm

# Compile .c files to .o files (creating the obj directory if needed)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# 'tests' target: build and run the test executable.
test: $(TEST_EXE)
	./$(TEST_EXE)

# Link the test executable with the required object files
$(TEST_EXE): $(TEST_OBJ) $(OBJ_DIR)/parser.o $(OBJ_DIR)/spreadsheet.o
	$(CC) -o $(TEST_EXE) $(TEST_OBJ) $(OBJ_DIR)/parser.o $(OBJ_DIR)/spreadsheet.o -lm

# Clean up object files and executables
clean:
	rm -rf $(OBJ_DIR) $(TARGET_DIR) $(TEST_EXE)

# New target to compile LaTeX report
# This compiles latex_source/main.tex into main.pdf in the current directory
report:
	pdflatex -interaction=nonstopmode -halt-on-error -output-directory=. latex_source/main.tex
	pdflatex -interaction=nonstopmode -halt-on-error -output-directory=. latex_source/main.tex

.PHONY: all test clean report
