#ifndef SPRITELIGHT_H
#define SPRITELIGHT_H

#include "shader.h"
#include "texture.h"
#include "lights.h"
#pragma once

static SDL_Event window_event;
static SDL_Window *window;
static SDL_GLContext context;

static unsigned int g_buffer, g_position, g_normal, g_albedo, post_process_FBO, post_process_color, rboDepth;
static float scene_exposure = 1;
static Shader geometry_shader, basic, advanced,
    color_shader, circle_shader, downsample_shader,
    upsample_shader, post_process_shader, sky_shader;

static float x1 = 0; // points for line (controlled by mouse)
static float y_1 = 0;
static float x2 = 0; // static point
static float y2 = 0;

static float sx = 200; // square position
static float sy = 100;
static float sw = 200; // and size
static float sh = 200;

static float intersectionX = 0;
static float intersectionY = 0;

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

static float plane_vertices[] = {
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

static float line_vertices[] = {
    // position
    0,
    0,
    0,
    0,
    10,
    0,
};

static float cube_vertices[] = {
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

static unsigned int VBO = 0;
static unsigned int planeVBO = 0, planeVAO = 0;
static unsigned int quadVBO = 0, quadVAO = 0;
static unsigned int lineVBO = 0, lineVAO = 0;
static unsigned int cubeVBO = 0, cubeVAO = 0;

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

void paintest();

static float frame_time;
static unsigned int last_frame = 0;
static unsigned int current_frame;

static int screen_width = 1920;
static int screen_height = 1080;

typedef struct Camera
{
    Vector3 position;
    float angle;
    float fov;
    float zoom;
} Camera;
static float next_zoom = 1;

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

typedef struct BloomMip
{
    vec2 size;
    ivec2 int_size;
    Texture texture;
} BloomMip;
typedef struct Bloom
{
    unsigned int mip_chain_len;
    BloomMip *mip_chain;
    unsigned int FBO;
    bool karis_average;
    bool enabled;
} Bloom;
static Bloom bloom;
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
    bool fullscreen;
} State;
static State state;
static Texture cube;
static Collider Boxes[10];
static float frame_time;

static mat4 projection, model, view;

int GetRandomValue(int min, int max);

float Lerp(float start, float end, float amount);

Rectangle *CreateRectangle(int x, int y, int width, int height, Texture tex);

Collider RecToCollider(Rectangle rec, bool rotating, bool dynamic);

void DrawQuad();

void DrawLine(Vector2 line_start, Vector2 line_end, vec4 color);

void DrawRectangleBasic(Rectangle rec, vec4 color);

void DrawEntity(Rectangle rec);

void DrawCube(vec3 position, vec3 scale, Vector3 rotation);

void DrawRectangle(Rectangle rec);

// Draw a circle to a quad
void DrawCircle(vec3 position, float radius, vec4 color);

Vector3 Vector3Transform(vec3 v, mat4 mat);

Vector2 GetScreenToWorld2D(Vector2 position);

void OnResize(int new_width, int new_height);
void LightingPass();
void engine_init(char *window_name, int width, int height, int bloom_strength);
#endif // SPRITELIGHT_H