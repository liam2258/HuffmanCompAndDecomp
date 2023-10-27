# Makefile for building the main executable

# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -g

# Source files
SOURCES = src/main.cpp src/BitUtils.cpp src/CompUtils.cpp src/MapUtils.cpp src/Node.cpp src/TreeUtils.cpp

# Executable name
EXECUTABLE = main

# Build target
all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $@ $(SOURCES)

clean:
	rm -f $(EXECUTABLE)