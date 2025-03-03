CXX = g++
CXXFLAGS = -std=c++17 -Wall
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system

all: chess

chess: chess.cpp gameplay.cpp gui.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(SFML_LIBS)

.PHONY: clean
clean:
	rm -f chess
