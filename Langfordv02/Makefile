# Makefile for main.c

# Compiler and flags
CXX := gcc
CXXFLAGS := -Wall -Wextra -Ofast -std=c17 -L/libm-install-dir/lib -lm
LDFLAGS := -ltbb  # Link the Intel TBB library

# Target executable
TARGET := main 

# Source files
SRC := main.c langford.c

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