CXX      := clang++
CXXFLAGS := -std=c++20 -Wall -Wextra -Wpedantic -O2 -g
CPPFLAGS := -Iinclude -I../telemetry_lib/include

SRCS := src/TelemetryFrame.cpp src/TelemetrySimulator.cpp main.cpp
OBJS := $(SRCS:.cpp=.o)
TARGET := telemetry_simulator

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -L../telemetry_lib -ltelemetry -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@
