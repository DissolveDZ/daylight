#ifndef SPRITELIGHT_H
#define SPRITELIGHT_H

#include "shader.h"
#include "texture.h"
#include "lights.h"
#pragma once

static float scene_exposure = 1;
static Shader circle_shader, downsample_shader, color_shader, upsample_shader;

static float intersectionX = 0;
static float intersectionY = 0;
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
static int MAX_BLOOM_MIP = 10;
static mat4 model, view;

int GetRandomValue(int min, int max);

float Lerp(float start, float end, float amount);

Rectangle *CreateRectangle(int x, int y, int width, int height, Texture tex);

Collider RecToCollider(Rectangle rec, bool rotating, bool dynamic);

void DrawQuad();

void DrawLine(Vector2 line_start, Vector2 line_end, vec4 color);

void DrawRectangleBasic(Rectangle rec, vec4 color, Shader shader);

void DrawEntity(Rectangle rec, Shader shader);

void DrawCube(vec3 position, vec3 scale, Vector3 rotation, Texture texture, Shader shader);

void DrawRectangle(Rectangle rec, Shader shader);

// Draw a circle to a quad
void DrawCircle(vec3 position, float radius, vec4 color);

Vector3 Vector3Transform(vec3 v, mat4 mat);

Vector2 GetScreenToWorld2D(Vector2 position, mat4 projection);
void GBufferSetup(unsigned int *g_buffer, unsigned int *g_position, unsigned int *g_normal, unsigned int *g_albedo, unsigned int *depth);
void PostProcessBuffer(unsigned int *post_process_fbo, unsigned int *post_process_color, unsigned int *depth);
void BufferSetup(unsigned int *VAO, unsigned int *VBO, float vertices[], int size, bool textured, bool normals);
void OnResize(int new_width, int new_height);
void LightingPass();
void engine_init(char *window_name, int width, int height, int bloom_strength);
void BloomInit(int mip_amount, Bloom *bloom);
void UpsampleBloom(float filter_radius, Bloom *bloom, unsigned int *quadVAO);
void DownSampleBloom(unsigned int src_texture, float threshold, float knee, Bloom *bloom, unsigned int *quadVAO);
void RenderBloom(unsigned int src_texture, float filter_radius, float threshold, float knee, Bloom *bloom, unsigned int *quadVAO);

#endif // SPRITELIGHT_H