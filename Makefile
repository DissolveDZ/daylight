all:
	gcc src/main.c -Iinclude -lSDL2 -ggdb -o daylight -lm -DSDL_MAIN_HANDLED

run: all
	./daylight

windows:
	gcc src/main.c -Iinclude -lSDL2 -ggdb -o daylight -lm -DSLDL_MAIN_HANDLED