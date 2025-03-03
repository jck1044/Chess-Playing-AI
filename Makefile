CXX = g++
CXXFLAGS = -std=c++17 -Wall
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system

# Build both the main chess executable and the unit test executable.
all: chess test

chess: chess.cpp gameplay.cpp gui.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(SFML_LIBS)

test: test.cpp chess.cpp gameplay.cpp gui.cpp
	$(CXX) $(CXXFLAGS) -DUNIT_TEST -o $@ $^ $(SFML_LIBS)

.PHONY: clean run-tests

clean:
	rm -f chess test

run-tests: test
	./test
