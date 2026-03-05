# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Iinclude -Wall -Wextra

# Directories
SRC_DIR = src
OBJ_DIR = build
BIN = baddie_server69

# Find all .cpp files recursively
SRCS = $(shell find $(SRC_DIR) -name '*.cpp')

# Generate .o paths by replacing src/ with build/ and .cpp with .o
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Default target
all: $(BIN)

# Link
$(BIN): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Rule to compile each .cpp to corresponding .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build
clean:
	rm -rf $(OBJ_DIR) $(BIN)

.PHONY: all clean
