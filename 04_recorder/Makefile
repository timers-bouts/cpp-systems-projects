CXX      = clang++
CXXFLAGS = -std=c++20 -Wall -Wextra -Wpedantic \
           -Iinclude -I../telemetry_lib/include

TARGET   = recorder_demo

SRC      = main.cpp \
           ../telemetry_lib/src/TelemetryRecorder.cpp \
           ../telemetry_lib/src/PacketWriter.cpp

OBJ      = $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

rebuild: clean all
