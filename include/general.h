#include <math.h>
#include <cglm/call.h>
#include <stdio.h>
#include <stdlib.h>
#pragma once

char *ReadTextFile(char *path)
{
    void *file;
    long fsize;
    file = fopen(path, "rb");
    fseek(file, 0, SEEK_END);
    fsize = ftell(file);
    rewind(file);
    path = malloc(fsize + 1);
    fread(path, 1, fsize, file);
    path[fsize] = 0;
    return path;
}