# 05_reader/Makefile
#
# Project 5 — Telemetry Packet Reader Demo
#
# Builds:
#   - main.cpp (this project)
#   - telemetry_lib sources (shared reusable library)
#
# Usage:
#   make
#   make run ARGS=../04_recorder/output.bin
#   make clean

CXX      := clang++
CXXFLAGS := -std=c++20 -Wall -Wextra -Wpedantic -O2 -g

# Include path for shared telemetry headers
CPPFLAGS := -I../telemetry_lib/include

# Output directories
BUILD_DIR := build
OBJ_DIR   := $(BUILD_DIR)/obj
BIN_DIR   := $(BUILD_DIR)/bin

TARGET := $(BIN_DIR)/reader_demo

# Sources
APP_SRC := main.cpp
LIB_SRC := $(wildcard ../telemetry_lib/src/*.cpp)

SRC := $(APP_SRC) $(LIB_SRC)

# Object files
OBJ := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRC))

.PHONY: all clean run dirs

all: $(TARGET)

dirs:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/../telemetry_lib/src
	@mkdir -p $(BIN_DIR)

# Link final executable
$(TARGET): dirs $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $@

# Compile object files
$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

# Run helper
run: all
	./$(TARGET) $(ARGS)

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)
