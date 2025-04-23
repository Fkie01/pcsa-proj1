
# Makefile for building a simple terminal (shell) in C++

CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17
TARGET = mysh
SRC = main.cpp parser.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
