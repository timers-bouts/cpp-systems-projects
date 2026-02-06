# Compiler to use
CXX = clang++

# Compiler flags (options)
CXXFLAGS = -std=c++20 -Wall -Wextra -Iinclude

# Default target: what happens when you just type "make"
all: logger

# Build the logger executable
logger: main.cpp src/Logger.cpp
	$(CXX) $(CXXFLAGS) main.cpp src/Logger.cpp -o logger

# Clean up compiled files
clean:
	rm -f logger
