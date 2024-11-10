<<<<<<< HEAD
# Makefile for main.cpp

# Compiler and flags
CXX := g++
CXXFLAGS := -Wall -Wextra -Ofast -std=c++17 -L/libm-install-dir/lib -lm
LDFLAGS := -ltbb  # Link the Intel TBB library

# Target executable
TARGET := main

# Source files
SRC := main.cpp

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
=======
# Nom de l'exécutable
TARGET = main

# Compilateur
CXX = g++

# Options de compilation
CXXFLAGS = -Wall -Wextra -O3 -std=c++17

# Fichiers sources et objets
SRCS = main.cpp
OBJS = $(SRCS:.cpp=.o)

# Règle par défaut : compile l'exécutable
all: $(TARGET)

# Règle pour créer l'exécutable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Règle pour compiler les fichiers .cpp en .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Règle pour nettoyer les fichiers compilés
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets pour éviter des conflits avec des fichiers du même nom
.PHONY: all clean
>>>>>>> cc7eb6160841baafd80b0e44f56113ae711be90b
