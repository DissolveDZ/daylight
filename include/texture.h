#include <math.h>
#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stb_image.h"
#include <math.h>

#pragma once

typedef struct Texture
{
    unsigned int ID;
    char *path;
    char *type;
    int width;
    int height;
} Texture;

static inline Texture LoadTexture2D(const char *path, float anisotropy, bool gamma)
{
    Texture texture;
    glGenTextures(1, &texture.ID);
    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum internal_format;
        GLenum data_format;

        if (nrComponents == 1)
            internal_format = data_format = GL_RED;
        else if (nrComponents == 3)
        {
            internal_format = gamma ? GL_SRGB : GL_RGB;
            data_format = GL_RGB;
        }
        else if (nrComponents == 4)
        {
            internal_format = gamma ? GL_SRGB_ALPHA : GL_RGBA;
            data_format = GL_RGBA;
        }
        texture.width = width;
        texture.height = height;
        glBindTexture(GL_TEXTURE_2D, texture.ID);
        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, data_format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        if (anisotropy != 0)
        {
            GLfloat value, max_anisotropy = anisotropy;
            glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &value);
            value = (value > max_anisotropy) ? max_anisotropy : value;
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, value);
        }
    }
    else
        printf("failed loading texture\n");
    stbi_image_free(data);
    return texture;
}

static inline void BindTexture(int ID)
{
    // idk what i tried doing here
}

static inline void Render2DTexture(Texture texture)
{
    // make quad
}