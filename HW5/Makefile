# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -Wall
LDFLAGS := -lm
INCLUDES := -Iinclude  # Add your include directory here

# Directories
SRC_DIR := src
OBJ_DIR := obj

# File extensions
SRC_EXT := cpp
OBJ_EXT := o

# Source and Object files
SRCS := $(wildcard $(SRC_DIR)/*.$(SRC_EXT))
OBJS := $(patsubst $(SRC_DIR)/%.$(SRC_EXT),$(OBJ_DIR)/%.$(OBJ_EXT),$(SRCS))

# Executable name
TARGET := test

# Default target
all: $(TARGET)

# Linking
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Compilation
$(OBJ_DIR)/%.$(OBJ_EXT): $(SRC_DIR)/%.$(SRC_EXT) | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@ $(LDFLAGS)

# Create obj directory
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Phony target to clean the project
.PHONY: clean
clean:
	rm -rf $(OBJ_DIR) $(TARGET)
