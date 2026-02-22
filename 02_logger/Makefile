.PHONY: all clean rebuild run

CXX      := clang++
CXXFLAGS := -std=c++20 -Wall -Wextra -Wpedantic -g
CPPFLAGS := -Iinclude -I../telemetry_lib/include

BUILD_DIR := build
BIN_DIR   := $(BUILD_DIR)/bin
OBJ_DIR   := $(BUILD_DIR)/obj
TARGET    := $(BIN_DIR)/logger_test

APP_SRC := main.cpp
LIB_SRC := ../telemetry_lib/src/Logger.cpp

APP_OBJ := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(APP_SRC))
LIB_OBJ := $(patsubst ../telemetry_lib/src/%.cpp,$(BUILD_DIR)/telemetry_lib/src/%.o,$(LIB_SRC))

OBJ := $(APP_OBJ) $(LIB_OBJ)

all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ)

# Project sources -> build/obj/
$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

# Shared library sources -> build/telemetry_lib/src/
$(BUILD_DIR)/telemetry_lib/src/%.o: ../telemetry_lib/src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

run: all
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR)

rebuild: clean all
