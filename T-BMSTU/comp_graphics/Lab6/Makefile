NAME=main
SRC=src/main.cpp
CFLAGS=-g -lGL -lGLEW -lglfw -lSOIL -Wall -o
CC=g++

default: build run

run:
	./$(NAME)

build: $(SRC)
	$(CC) $(CFLAGS) $(NAME) $(SRC)

clean:
	rm -f $(NAME)
