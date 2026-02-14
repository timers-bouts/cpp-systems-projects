# ----------------------------
# Compiler + Flags
# ----------------------------

CXX       = clang++
CXXFLAGS  = -std=c++20 -Wall -Wextra -Wpedantic -Wconversion -Iinclude
LDFLAGS   =

# ----------------------------
# Project Structure
# ----------------------------

SRC_DIR   = src
OBJ_DIR   = build
BIN_DIR   = bin

TARGET    = $(BIN_DIR)/packet_demo

SOURCES   = $(SRC_DIR)/PacketWriter.cpp main.cpp
OBJECTS   = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

# ----------------------------
# Build Rules
# ----------------------------

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJECTS) -o $(TARGET)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ----------------------------
# Convenience Targets
# ----------------------------

run: all
	./$(TARGET)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

format:
	clang-format -i $(SRC_DIR)/*.cpp include/telemetry/*.h main.cpp

.PHONY: all run clean format
