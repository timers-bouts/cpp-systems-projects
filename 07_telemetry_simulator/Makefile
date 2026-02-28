.PHONY: all clean rebuild run

CXX      := clang++
CXXFLAGS := -std=c++20 -Wall -Wextra -Wpedantic -O2 -g
CPPFLAGS := -Iinclude -I../telemetry_lib/include -I../telemetry_sim/include

BUILD_DIR := build
BIN_DIR   := $(BUILD_DIR)/bin
OBJ_DIR   := $(BUILD_DIR)/obj
TARGET    := $(BIN_DIR)/simulator_demo

APP_SRC := main.cpp
# Explicit list of all source files to compile
LIB_SRC := ../telemetry_lib/src/TelemetryRecorder.cpp \
           ../telemetry_lib/src/TelemetryReader.cpp \
		    ../telemetry_lib/src/PacketWriter.cpp \
			../telemetry_lib/src/PacketReader.cpp \
			../telemetry_lib/src/Logger.cpp \
           ../telemetry_lib/src/CRC.cpp \
           src/TelemetrySimulator.cpp \
           src/TelemetryFrame.cpp \

APP_OBJ := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(APP_SRC))
LIB_OBJ := \
    $(patsubst ../telemetry_lib/src/%.cpp,$(BUILD_DIR)/telemetry_lib/src/%.o,$(filter ../telemetry_lib/src/%.cpp,$(LIB_SRC))) \
    $(patsubst src/%.cpp,$(BUILD_DIR)/telemetry_sim/src/%.o,$(filter src/%.cpp,$(LIB_SRC)))

OBJ := $(APP_OBJ) $(LIB_OBJ)

all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ)

# Project sources -> build/obj/
$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

# telemetry_lib sources -> build/telemetry_lib/src/
$(BUILD_DIR)/telemetry_lib/src/%.o: ../telemetry_lib/src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

# telemetry_sim sources -> build/telemetry_sim/src/
$(BUILD_DIR)/telemetry_sim/src/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

run: all
	./$(TARGET) $(ARGS)

clean:
	rm -rf $(BUILD_DIR)

rebuild: clean all
