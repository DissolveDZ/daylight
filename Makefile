all:
	gcc -E -ISpriteLight/engine_include SpriteLight/engine_include/SpriteLight.h -o SpriteLight/engine_include/engine.h
	cp SpriteLight/engine_include/engine.h include
	gcc SpriteLight/src/main.c -shared -ISpriteLight/engine_include -ISpriteLight/include -fPIC -lSDL2main -lSDL2 -lmingw32 -lm -o bin/libSpriteLight.dll -D_FILE_OFFSET_BITS=64 -ggdb
	gcc src/main.c bin/libSpriteLight.dll -o bin/daylight -Iinclude -lmingw32 -lSDL2main -lSDL2 -lm -ggdb
run: all
	./daylight

static:
	gcc -E -ISpriteLight/engine_include SpriteLight/engine_include/SpriteLight.h -o SpriteLight/engine_include/engine.h
	cp SpriteLight/engine_include/engine.h include
	gcc -c SpriteLight/src/main.c -o bin/libSpriteLight.o -ISpriteLight/include -lSDL2main -DSLDL_MAIN_HANDLED -Wl,-Bdynamic -lopengl -Wl,-Bstatic -lSDL2 -lmingw32 -lm -D_FILE_OFFSET_BITS=64 -ggdb
	ar rcs bin/libSpriteLight.a bin/libSpriteLight.o
	gcc -static-libgcc src/main.c bin/libSpriteLight.o -Iinclude -L. -lSDL2 -lSpriteLight -lm -ggdb -Wall -o bin/daylight
	./bin/daylight

header:
	gcc -E -ISpriteLight/engine_include SpriteLight/engine_include/SpriteLight.h -o SpriteLight/engine_include/engine.h
	cp SpriteLight/engine_include/engine.h include

win:
	gcc src/main.c -Iinclude -lSDL2 -ggdb -o daylight -lm -DSLDL_MAIN_HANDLED