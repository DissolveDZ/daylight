all:
	clang src/main.c -Iinclude -lGL -lSDL2 -ggdb -o weed -lm -fsanitize=undefined

run: all
	./weed