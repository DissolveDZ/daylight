#define SDL_MAIN_HANDLED
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <win_include.h>
#include <glad/glad.h>
#include "container.h"
#include "def.h"
#include "init.c"
#include "collisions.c"
#include "update.c"
#include "draw.c"

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
    SDL_GL_DeleteContext(main_context);
    SDL_DestroyWindow(main_window);
    SDL_Quit();
    return 0;
}