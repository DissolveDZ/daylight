#include <stdlib.h>
#include <stdio.h>
#ifdef _WIN32
#include <win/SDL2/SDL.h>
#include <win/cglm/call.h>
#else
#include <SDL2/SDL.h>
#include <cglm/call.h>
#endif
#include <glad/glad.h>
#include "container.h"

Texture tex;

SDL_Event window_event;
SDL_Window *main_window;
SDL_GLContext main_context;

mat4 projection;

unsigned int g_buffer, g_position, g_normal, g_albedo, post_process_fbo, post_process_color, depth_rbo;

Shader geometry_shader, basic, advanced, post_process_shader, sky_shader;

static float quad_vertices[] = {
    // positions        // texture Coords
    -1.0f,
    1.0f,
    0.0f,
    0.0f,
    1.0f,
    -1.0f,
    -1.0f,
    0.0f,
    0.0f,
    0.0f,
    1.0f,
    1.0f,
    0.0f,
    1.0f,
    1.0f,
    1.0f,
    -1.0f,
    0.0f,
    1.0f,
    0.0f,
};
float x1 = 0; // points for line (controlled by mouse)
float y_1 = 0;
float x2 = 0; // static point
float y2 = 0;

float sx = 200; // square position
float sy = 100;
float sw = 200; // and size
float sh = 200;

float frame_time;
unsigned int last_frame = 0;
unsigned int current_frame;
Collider Boxes[10];


float plane_vertices[] = {
    // positions        // texture Coords
    -0.5f,
    0.5f,
    0.0f,
    0.0f,
    1.0f,
    -0.5f,
    -0.5f,
    0.0f,
    0.0f,
    0.0f,
    0.5f,
    0.5f,
    0.0f,
    1.0f,
    1.0f,
    0.5f,
    -0.5f,
    0.0f,
    1.0f,
    0.0f,
};

float line_vertices[] = {
    // position
    0,
    0,
    0,
    0,
    10,
    0,
};

float cube_vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

    -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};

unsigned int VBO = 0;
unsigned int planeVBO = 0, planeVAO = 0;
unsigned int lineVBO = 0, lineVAO = 0;
unsigned int cubeVBO = 0, cubeVAO = 0;
unsigned int quadVBO = 0, quadVAO = 0;

void DrawQuad()
{
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

Texture cube;
Bloom bloom;

#define MAX_BUILDINGS 100

Rectangle buildings[100] = {0};
vec4 buildColors[100] = {0};

PointLight *pight;
PointLight *point;

void OnResize(int new_width, int new_height)
{
    if (new_width > (int)INT_MAX || new_height > (int)INT_MAX)
    {
        printf("Window size overflow");
        return false;
    }
    screen_width = new_width;
    screen_height = new_height;
    glBindTexture(GL_TEXTURE_2D, g_position);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screen_width, screen_height, 0, GL_RGBA, GL_FLOAT, NULL);
    glBindTexture(GL_TEXTURE_2D, g_normal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screen_width, screen_height, 0, GL_RGBA, GL_FLOAT, NULL);
    glBindTexture(GL_TEXTURE_2D, g_albedo);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screen_width, screen_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, post_process_color);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screen_width, screen_height, 0, GL_RGBA, GL_FLOAT, NULL);
    glBindRenderbuffer(GL_RENDERBUFFER, depth_rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screen_width, screen_height);
    glViewport(0, 0, screen_width, screen_height);
    if (bloom.enabled)
    {
        vec2 mip_size = {(float)screen_width, (float)screen_height};
        ivec2 mip_int_size = {(int)screen_width, (int)screen_height};
        for (unsigned int i = 0; i < bloom.mip_chain_len; i++)
        {
            BloomMip *mip = &bloom.mip_chain[i];
            glm_vec2_mul(mip_size, (vec2){0.5f, 0.5f}, mip_size);
            glm_vec2_div(mip_int_size, (ivec2){2, 2}, mip_int_size);
            glm_vec2_copy(mip_size, mip->size);
            glm_ivec2_copy(mip_int_size, mip->int_size);
            glBindTexture(GL_TEXTURE_2D, mip->texture.ID);
            // we are downscaling an HDR color buffer, so we need a float texture format
            glTexImage2D(GL_TEXTURE_2D, 0, GL_R11F_G11F_B10F,
                         (int)mip_size[0], (int)mip_size[1],
                         0, GL_RGB, GL_FLOAT, NULL);
        }
    }
}

Collider RecToCollider(Rectangle rec, bool rotating, bool dynamic)
{
    Collider col;
    if (rotating)
    {
        col.vertices = malloc(sizeof(Line) * 2);
        col.vertices[0] = (Line){(Vector2){0, 0}, (Vector2){0 + rec.width, rec.y}};
        col.vertices[1] = (Line){(Vector2){0, rec.height}, (Vector2){rec.width, rec.height}};
    }
    col.x = rec.x;
    col.y = rec.y;
    col.width = rec.width;
    col.height = rec.height;
    col.rotating = rotating;
    col.dynamic = dynamic;
    state.cur_colliders++;
    return col;
}

Vector3 Vector3Transform(vec3 v, mat4 mat)
{
    Vector3 result;
    float x = v[0];
    float y = v[1];
    float z = v[2];
    result.x = mat[0][0] * x + mat[1][0] * y + mat[2][0] * z + mat[3][0];
    result.y = mat[0][1] * x + mat[1][1] * y + mat[2][1] * z + mat[3][1];
    result.z = mat[0][2] * x + mat[1][2] * y + mat[2][2] * z + mat[3][2];
    return result;
}

Vector2 GetScreenToWorld2D(Vector2 position, mat4 projection)
{
    Vector2 clipcoord = {2.0f * position.x / (float)screen_width - 1.0f, 1.0 - 2.0 * position.y / (float)screen_height};
    mat4 inv_view;
    glm_mat4_inv(projection, inv_view);
    Vector3 transform;
    // glm_scale(invmat, transform);
    transform = Vector3Transform((vec3){clipcoord.x, clipcoord.y, 1}, inv_view);
    // transform = (vec4){position.x, position.y, 1, 1} * inv_view;
    return (Vector2){transform.x * state.camera.position.z + state.camera.position.x, transform.y * state.camera.position.z + state.camera.position.y};
}