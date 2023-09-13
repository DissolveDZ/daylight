all:
	gcc -c SpriteLight/src/main.c -Iinclude -ISpriteLight/include -ggdb -o SpriteLight.o -lmingw32 -lSDL2main -lSDL2 -DSLDL_MAIN_HANDLED -static -shared
	ar rs lib/libSpriteLight.a SpriteLight.o
	gcc -Iinclude -c src/main.c -o daylight.o
	gcc -Iinclude src/main.c -lSDL2 -ggdb -o daylight -Llib -lSpriteLight
	rm *.o
run: all
	./daylight

windows:
	gcc src/main.c -Iinclude -lSDL2 -ggdb -o daylight -lm -DSLDL_MAIN_HANDLED