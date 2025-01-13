# Makefile for langfordv01.c

# Compiler and flags
CXX := gcc
CXXFLAGS := -Wall -Wextra -Ofast -std=c17 -L/libm-install-dir/lib -lm
LDFLAGS := -ltbb  # Link the Intel TBB library

# Target executable
TARGET := langfordv01

# Source files
SRC := langfordv01.c 

# Object files
OBJ := $(SRC:.c=.o)

# Default target to build the executable
all: $(TARGET)

# Rule to link the executable
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Rule to compile source files to object files
%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJ) $(TARGET)

# Phony targets (to avoid conflicts with file names)
.PHONY: all clean