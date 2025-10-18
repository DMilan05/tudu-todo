# --- TUDU TODO GTK4 PROJECT MAKEFILE ---

# Compiler & flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude $(shell pkg-config --cflags gtk4)
LDFLAGS = $(shell pkg-config --libs gtk4)

# Directories
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build
TEST_DIR = tests

# Output
TARGET = tudu_todo

# Source files
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC))

# Default target
all: $(BUILD_DIR) $(TARGET)

# Link all objects
$(TARGET): $(OBJ)
	@echo "Linking $(TARGET)..."
	$(CC) $(OBJ) -o $@ $(LDFLAGS)
	@echo "Build complete!"

# Compile each .c file into .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Create build directory if missing
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Run app
run: $(TARGET)
	./$(TARGET)

# Clean build files
clean:
	rm -rf $(BUILD_DIR) $(TARGET)
	@echo "Cleaned!"

# Build & run
rebuild: clean all run
