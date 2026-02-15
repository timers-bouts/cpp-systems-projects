.PHONY: all clean rebuild run

CXX      = clang++
CXXFLAGS = -std=c++20 -Wall -Wextra -Wpedantic -O2 -g
CPPFLAGS = -Iinclude -I../telemetry_lib/include

# Output directories
BUILD_DIR = build
BIN_DIR   = bin

# Final executable name + location
TARGET = $(BIN_DIR)/recorder_demo

# Sources
SRC = main.cpp \
      ../telemetry_lib/src/TelemetryRecorder.cpp \
      ../telemetry_lib/src/PacketWriter.cpp

# Object files go into build/ mirroring relative paths
OBJ = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SRC))

all: $(TARGET)

# Link final executable
$(TARGET): $(OBJ)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ)

# Compile .cpp -> build/.../.o
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

run: all
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

rebuild: clean all
