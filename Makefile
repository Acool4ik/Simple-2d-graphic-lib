CC=g++
CFLAGS=-Wall -Wextra -Wswitch-enum -pedantic -std=gnu++17
CFLAGS_DEBUG=-fsanitize=address -ggdb -pg -g3 
CFLAGS_RELEASE=-O3 -g0
LFLAGS=-lpng

IMG=./result.png
SRC=./src/main.cpp ./src/rastrum_image.cpp ./src/shapes.cpp
INC=./inc/rastrum_image.hpp ./inc/shapes.hpp ./inc/backtrack.hpp
OBJ=$(SRC:.cpp=.o)

build_release: $(SRC) $(INC)
	$(CC) $(CFLAGS) $(CFLAGS_RELEASE) -o main $(SRC) $(LFLAGS)

build_debug: $(SRC) $(INC)
	$(CC) $(CFLAGS) $(CFLAGS_DEBUG) -o main $(SRC) $(LFLAGS)

clean:
	-rm $(OBJ) $(IMG) ./main 2>/dev/null
