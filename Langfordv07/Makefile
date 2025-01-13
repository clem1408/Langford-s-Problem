# Makefile for langfordv07.cpp

# Compiler and flags
# mpicc
CXX := mpic++
CXXFLAGS := -Wall -Wextra -Ofast -std=c++17 -Wcast-function-type
LDFLAGS := -ltbb -L/libm-install-dir/lib -lm # Link the Intel TBB library

# Target executable
TARGET := langfordv07

# Source files
SRC := langfordv07.cpp

# Object files
OBJ := $(SRC:.cpp=.o)

# Default target to build the executable
all: $(TARGET)

# Rule to link the executable
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Rule to compile source files to object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJ) $(TARGET)

# Phony targets (to avoid conflicts with file names)
.PHONY: all clean
