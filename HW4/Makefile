# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++11 -Wall

# Executable name
EXECUTABLE := test

# Source files
SRC := $(wildcard *.cpp)
DEPS := $(wildcard *.cpp)

# Object files
OBJ := $(SRC:.cpp=.o)

# Targets
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(EXECUTABLE) $(OBJ)

%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXECUTABLE)
