#pragma once

typedef struct Texture
{
    unsigned int ID;
    char *path;
    char *type;
    int width;
    int height;
} Texture;

Texture LoadTexture2D(const char *path, float anisotropy, bool gamma);