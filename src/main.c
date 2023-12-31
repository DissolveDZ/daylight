#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "glad.c"
#include <glad/glad.h>
#include <cglm/call.h>
#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "shader.h"
#include "lights.h"
#include "main.h"
#include "init.c"
#include "update.c"
#include "draw.c"

State state;
int main(void)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    Init();
    while (!state.quit)
    {
        Update();
        Draw();
    }
    glDeleteVertexArrays(1, &planeVAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(basic.ID);
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}