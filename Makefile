CC=g++
CFLAGS=-Wall -Wextra -Wswitch-enum -pedantic -O3 -fsanitize=address -std=gnu++17
LFLAGS=-lpng

SRC=./src/main.cpp ./src/rastrum_image.cpp ./src/shapes.cpp ./src/backtrack.cpp
INC=./inc/rastrum_image.hpp ./inc/shapes.hpp ./inc/backtrack.hpp

target: $(SRC) $(INC)
	$(CC) $(CFLAGS) -o main $(SRC) $(LFLAGS)
