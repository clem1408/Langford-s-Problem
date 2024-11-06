# Nom de l'exécutable
TARGET = main

# Compilateur
CXX = g++

# Options de compilation
CXXFLAGS = -Wall -Wextra -O3 -std=c++17 -fopenmp

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
