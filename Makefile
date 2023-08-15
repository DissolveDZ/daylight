all:
	gcc src/main.c -Iinclude -lSDL2 -ggdb -o weed -lm -DSDL_MAIN_HANDLED

run: all
	./weed

windows:
	gcc src/main.c -Iinclude -lSDL2 -ggdb -o weed -lm -DSLDL_MAIN_HANDLED