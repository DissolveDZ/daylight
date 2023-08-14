#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <cglm/call.h>
#include "shader.h"
#include "texture.h"
#include "lights.h"
#pragma once

SDL_Event window_event;
SDL_Window *window;
SDL_GLContext context;

Shader basic;
Shader advanced;
Shader color_shader;

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
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

    -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f};

unsigned int VBO = 0;
unsigned int planeVBO = 0, planeVAO = 0;
unsigned int lineVBO = 0, lineVAO = 0;
unsigned int cubeVBO = 0, cubeVAO = 0;
Texture tex;
Texture cube;

typedef struct Vector2
{
    float x;
    float y;
} Vector2;

typedef struct Vector3
{
    float x;
    float y;
    float z;
} Vector3;

float frame_time;
unsigned int last_frame = 0;
unsigned int current_frame;

int screen_width = 1920;
int screen_height = 1080;

mat4 projection;

#define MAX_BUILDINGS 100

typedef struct Camera
{
    Vector3 position;
    float angle;
    float fov;
    float zoom;
} Camera;
float next_zoom = 1;

typedef struct Material
{

} Material;

typedef struct Line
{
    Vector2 start;
    Vector2 end;
} Line;

typedef struct Rectangle
{
    float x;
    float y;
    float width;
    float height;
    int texture_id;
    // Material mat;
} Rectangle;

typedef struct Collider
{
    float x;
    float y;
    float width;
    float height;
    Line *vertices;
    bool dynamic;
    bool rotating;
} Collider;

typedef struct Entity
{
    float accel;
    float decel;
    Vector2 velocity;
    Vector2 pelocity;
    bool is_floor;
    float speed;
    float max_speed;
    float jump_height;
    float health;
    bool flip;
    Texture tex;
    Collider col;
    Rectangle floor_check;
} Entity;

typedef struct PointIntersect
{
    Vector2 dist;
    bool hit;
} PointIntersect;

typedef enum Player_State
{
    IDLE,
    WALKING
} Player_State;

typedef struct Player
{
    float turn_mult;
    float vertical_to_horizontal;
    Vector2 last_velocity;
    bool was_floor;
    Entity entity;
    Player_State state;
} Player;

typedef struct State
{
    Player player;
    Camera camera;
    float gravity;
    Vector2 mouse_world;
    bool quit;
    unsigned int max_colliders;
    unsigned int cur_colliders;
    mat4 *matrices;
    unsigned int recs_len, recs_max;
    Rectangle **recs;
} State;
extern State state;

mat4 projection, model, view;

Rectangle buildings[MAX_BUILDINGS] = {0};
vec4 buildColors[MAX_BUILDINGS] = {0};

Collider Boxes[10];
PointLight *pight;
PointLight *point;
// TrueRectangle testing;

int GetRandomValue(int min, int max)
{
    if (min > max)
    {
        int tmp = max;
        max = min;
        min = tmp;
    }
    if ((unsigned int)(max - min) > (unsigned int)RAND_MAX)
    {
        printf("value exceeds %i", RAND_MAX);
    }
    return (rand() % (abs(max - min) + 1) + min);
}

float Lerp(float start, float end, float amount)
{
    float result = start + amount * (end - start);

    return result;
}

Rectangle *CreateRectangle(int x, int y, int width, int height, Texture tex)
{
    Rectangle *rec = malloc(sizeof(Rectangle));
    *rec = (Rectangle){x, y, width, height, tex.ID};
    if (state.recs_len + 1 > state.recs_max)
    {
        state.recs_max += 20;
        state.recs = realloc(state.recs, state.recs_max);
    }
    state.recs[state.recs_len] = rec;
    state.recs_len++;
    return rec;
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

void DrawLine(Vector2 line_start, Vector2 line_end, vec4 color)
{
    UseShader(color_shader);

    SetShaderMat4(color_shader.ID, "projection", projection);
    SetShaderMat4(color_shader.ID, "view", view);
    SetShaderVec4(color_shader.ID, "color", (vec4){color[0] / 255, color[1] / 255, color[2] / 255, color[3] / 255});
    glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(line_vertices), line_vertices, GL_STATIC_DRAW);
    glm_mat4_identity(model);
    line_vertices[0] = line_start.x;
    line_vertices[1] = line_start.y;
    line_vertices[2] = 0;
    line_vertices[3] = line_end.x;
    line_vertices[4] = line_end.y;
    line_vertices[5] = 0;
    SetShaderMat4(color_shader.ID, "model", model);
    glBindVertexArray(lineVAO);
    glDrawArrays(GL_LINES, 0, 2);
    glBindVertexArray(0);
}

void DrawRectangleBasic(Rectangle rec, vec4 color)
{
    // Skip matrix when not moving
    UseShader(basic);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);

    SetShaderMat4(basic.ID, "projection", projection);
    SetShaderMat4(basic.ID, "view", view);
    SetShaderBool(basic.ID, "use_color", true);
    SetShaderBool(basic.ID, "use_normals", false);
    SetShaderVec4(basic.ID, "color", (vec4){color[0] / 255, color[1] / 255, color[2] / 255, color[3] / 255});

    glm_mat4_identity(model);
    glm_translate(model, (vec3){rec.x, rec.y, 0.0f});
    glm_scale(model, (vec3){rec.width, rec.height, 1});
    SetShaderMat4(basic.ID, "model", model);

    glBindVertexArray(planeVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}
void DrawEntity(Rectangle rec)
{
    // Skip matrix when not moving
    UseShader(basic);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, rec.texture_id);
    SetShaderMat4(basic.ID, "projection", projection);
    SetShaderMat4(basic.ID, "view", view);
    SetShaderBool(basic.ID, "use_color", false);
    SetShaderBool(basic.ID, "use_normals", false);
    SetShaderVec3(basic.ID, "view_pos", (vec3){state.camera.position.x, state.camera.position.y, state.camera.position.z});
    SetShaderBool(basic.ID, "flip", state.player.entity.flip);

    glm_mat4_identity(model);
    glm_translate(model, (vec3){rec.x, rec.y, 0.0f});
    glm_scale(model, (vec3){rec.width, rec.height, 1});
    SetShaderMat4(basic.ID, "model", model);

    glBindVertexArray(planeVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void DrawCube(vec3 position, vec3 scale, Vector3 rotation)
{
    glEnable(GL_DEPTH_TEST);
    UseShader(advanced);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, cube.ID);
    SetShaderMat4(advanced.ID, "projection", projection);
    SetShaderMat4(advanced.ID, "view", view);
    SetShaderBool(advanced.ID, "use_color", false);
    SetShaderBool(advanced.ID, "use_normals", true);
    SetShaderVec3(advanced.ID, "view_pos", (vec3){state.camera.position.x, state.camera.position.y, state.camera.position.z});

    glm_mat4_identity(model);
    mat4 temp;
    glm_mat4_identity(temp);
    versor quater;
    glm_quat_identity(quater);
    mat4 idendidy;
    glm_mat4_identity(idendidy);
    mat4 transformx;
    glm_mat4_identity(transformx);
    glm_rotate(transformx, glm_rad(rotation.x), (vec3){1.f, 0.f, 0.f});
    mat4 transformy;
    glm_mat4_identity(transformy);
    glm_rotate(transformy, glm_rad(rotation.y), (vec3){0.f, 1.f, 0.f});
    mat4 transformz;
    glm_mat4_identity(transformz);
    glm_rotate(transformz, glm_rad(rotation.z), (vec3){0.f, 0.f, 1.f});
    mat4 rotationmat;
    glm_mat4_identity(rotationmat);
    glm_mul(transformy, transformx, temp);
    glm_mul(temp, transformz, rotationmat);

    // glm_quat_from_vecs((vec3){0, 0, 0}, (vec3){glm_rad(rotation.y), glm_rad(rotation.x), glm_rad(rotation.z)}, quater);
    // glm_quat_mat4(quater, temp);
    glm_translate(model, position);
    glm_mat4_mul(model, rotationmat, model);
    glm_scale(model, scale);
    // glm_rotate(model, glm_rad((float)SDL_GetTicks64() / 50), rotation);
    SetShaderMat4(advanced.ID, "model", model);

    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDisable(GL_DEPTH_TEST);
}

void DrawRectangle(Rectangle rec)
{
    // Skip matrix when not moving
    UseShader(basic);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, rec.texture_id);

    SetShaderMat4(basic.ID, "projection", projection);
    SetShaderMat4(basic.ID, "view", view);
    SetShaderBool(basic.ID, "use_color", false);
    SetShaderBool(basic.ID, "use_normals", false);
    SetShaderVec3(basic.ID, "view_pos", (vec3){state.camera.position.x, state.camera.position.y, state.camera.position.z});

    glm_mat4_identity(model);
    glm_translate(model, (vec3){rec.x, rec.y, 0.0f});
    glm_scale(model, (vec3){rec.width, rec.height, 1});
    SetShaderMat4(basic.ID, "model", model);

    glBindVertexArray(planeVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
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

Vector2 GetScreenToWorld2D(Vector2 position)
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

void OnResize(int new_width, int new_height)
{
    glViewport(0, 0, new_width, new_height);
    screen_width = new_width;
    screen_height = new_height;
    // state.camera.offset = (Vector2){screen_width / 2.0f, screen_height / 2.0f};
}

/*

typedef struct State
{
    Player player;
    Camera2D camera;
    float gravity;
    Vector2 mouse_world;
    int max_colliders;
    int cur_colliders;
} State;
extern State state;

typedef struct TrueRectangle
{
    Line top;
    Line bottom;
    Vector2 pos;
    Vector2 size;
    float rotation;
} TrueRectangle;

void DrawTrueRectangleTex(TrueRectangle rec, Texture2D tex, Color color)
{
    rlBegin(RL_TRIANGLES);
    rlColor4ub(color.r, color.g, color.b, color.a);

    rlVertex2f(rec.top.start.x, rec.top.start.y);
    rlVertex2f(rec.bottom.start.x, rec.bottom.start.y);
    rlVertex2f(rec.top.end.x, rec.top.end.y);

    rlVertex2f(rec.top.end.x, rec.top.end.y);
    rlVertex2f(rec.bottom.start.x, rec.bottom.start.y);
    rlVertex2f(rec.bottom.end.x, rec.bottom.end.y);
    rlEnd();
    rlSetTexture(0);
}
*/
/*

void RotateRec(TrueRectangle *rec, float rotation, Vector2 origin)
{
    float sinRotation = sinf(rotation * DEG2RAD);
    float cosRotation = cosf(rotation * DEG2RAD);
    float x = rec->pos.x + rec->size.x / 2;
    float y = rec->pos.y + rec->size.y / 2;
    Vector2 orig = (Vector2){origin.x + rec->size.x / 2, origin.y + rec->size.y / 2};
    float dx = -orig.x;
    float dy = -orig.y;
    rec->top.start.x = x + dx * cosRotation - dy * sinRotation;
    rec->top.start.y = y + dx * sinRotation + dy * cosRotation;

    rec->top.end.x = x + (dx + rec->size.x) * cosRotation - dy * sinRotation;
    rec->top.end.y = y + (dx + rec->size.x) * sinRotation + dy * cosRotation;

    rec->bottom.start.x = x + dx * cosRotation - (dy + rec->size.y) * sinRotation;
    rec->bottom.start.y = y + dx * sinRotation + (dy + rec->size.y) * cosRotation;

    rec->bottom.end.x = x + (dx + rec->size.x) * cosRotation - (dy + rec->size.y) * sinRotation;
    rec->bottom.end.y = y + (dx + rec->size.x) * sinRotation + (dy + rec->size.y) * cosRotation;
}

float rotation;
Rectangle water;
Shader water_shader;
Texture2D water_noise;
Light pight;
Shader light_shader;
*/