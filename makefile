CXX=clang++
LIBS=-lSDL2 -lSDL2_ttf
FLAGS=--std=c++17 -g

TEST_SOURCE=src/*.cpp

test: $(TEST_SOURCE)
	[ -d ./bin/ ] || mkdir bin
	$(CXX) $(FLAGS) $(LIBS) $(TEST_SOURCE) -o bin/test
