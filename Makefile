# Makefile for building the main executable with test mode

# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -g

# Source files
SOURCES = src/main.cpp src/BitUtils.cpp src/CompUtils.cpp src/MapUtils.cpp src/Node.cpp src/TreeUtils.cpp
TEST_SOURCES = src/BitUtils.cpp src/CompUtils.cpp src/MapUtils.cpp src/Node.cpp src/TreeUtils.cpp Testing/UnitTests/BitUtils_tests.cpp Testing/UnitTests/TreeUtils_tests.cpp

# Executable names
EXECUTABLE = main
TEST_EXECUTABLE = test_main

# Build targets
all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $@ $(SOURCES)

test: $(TEST_EXECUTABLE)
	./$(TEST_EXECUTABLE)

$(TEST_EXECUTABLE): $(TEST_SOURCES)
	$(CXX) $(CXXFLAGS) -o $@ $(TEST_SOURCES)

clean:
	rm -f $(EXECUTABLE) $(TEST_EXECUTABLE)
