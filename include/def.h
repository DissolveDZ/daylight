#include <win/SDL2/SDL.h>
#include <win/cglm/call.h>
#include <glad/glad.h>

Texture tex;

#define MAX_BUILDINGS 100

Rectangle buildings[100] = {0};
vec4 buildColors[100] = {0};

PointLight *pight;
PointLight *point;