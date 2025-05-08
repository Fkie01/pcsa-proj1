
# Makefile for building a simple terminal (shell) in C++

CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17
TARGET = icsh
SRC = icsh.cpp parser.cpp execute.cpp signalHandle.cpp isRedirection.cpp jobs.cpp
OBJ = $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET)
