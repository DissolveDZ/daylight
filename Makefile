
header:
	gcc -E -ISpriteLight/engine_include SpriteLight/engine_include/SpriteLight.h -o SpriteLight/include/engine.h
	cp SpriteLight/include/engine.h include

all: header
	gcc SpriteLight/src/main.c -shared -ISpriteLight/engine_include -ISpriteLight/include -fPIC -lopengl32 -lSDL2main -lSDL2 -lmingw32 -lm -o libSpriteLight.dll -D_FILE_OFFSET_BITS=64 -ggdb
	gcc src/main.c libSpriteLight.dll -o daylight -Iinclude -lmingw32 -lSDL2main -lSDL2 -lm -ggdb -Wl,-Bdynamic -lopengl32 -Wl,-Bstatic 
run: all
	./daylight

static: header
	gcc -c SpriteLight/src/main.c -o libSpriteLight.o -ISpriteLight/include -lopengl32 -lSDL2main -DSLDL_MAIN_HANDLED -lmingw32 -lm -D_FILE_OFFSET_BITS=64 -ggdb
	ar rcs libSpriteLight.a libSpriteLight.o
	gcc -static-libgcc src/main.c libSpriteLight.o -Iinclude -L. -lSDL2 -Wl,-Bdynamic -lopengl32 -Wl,-Bstatic -lm -ggdb -Wall -o daylight
	./daylight

win:
	gcc src/main.c -Iinclude -lSDL2 -ggdb -o daylight -lm -DSLDL_MAIN_HANDLED