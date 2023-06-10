#CXX = g++
#CXXFLAGS = -std=c++11 -Wall
#SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system
#
#all: chess
#
#chess: chess.cpp
#	$(CXX) $(CXXFLAGS) -o $@ $^ $(SFML_LIBS)
#
#.PHONY: clean
#clean:
#	rm -f chess

CXX = g++
CXXFLAGS = -std=c++11 -Wall -arch arm64
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system

all: chess

chess: chess.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(SFML_LIBS)

.PHONY: clean
clean:
	rm -f chess
