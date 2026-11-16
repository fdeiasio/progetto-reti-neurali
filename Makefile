CXX     = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -Werror -I./include -I./raylib/include
LIBS    = ./raylib/lib/libraylib.a -lGL -lm -lpthread -ldl -lrt -lX11

all: bin/game

bin/game: src/game.cpp src/rendering.cpp src/core.cpp
	mkdir -p bin
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS)

run: bin/game
	./bin/game

train:
	@echo "train: not implemented yet"

.PHONY: all run train
