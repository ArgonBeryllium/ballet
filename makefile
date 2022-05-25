CXX=clang++
LIBS=-lSDL2
FLAGS=--std=c++17 -g

TEST_SOURCE=src/*.cpp

test: $(TEST_SOURCE)
	$(CXX) $(FLAGS) $(LIBS) $(TEST_SOURCE) -o bin/test
