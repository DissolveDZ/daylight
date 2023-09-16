# 0 "SpriteLight/engine_include/SpriteLight.h"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "SpriteLight/engine_include/SpriteLight.h"
# 1 "SpriteLight/engine_include/shader.h" 1
       

char *ReadTextFile(char *path);



static char* FormatShaderUniform(const char* uniform_name, int index);

typedef struct Shader
{
    unsigned int ID;
} Shader;

Shader LoadShader(char *vertex_path, char *fragment_path);

void UseShader(Shader shader);

void SetShaderBool(int Shader_ID, const char *name, bool value);

void SetShaderInt(int Shader_ID, const char *name, int value);

void SetShaderFloat(int Shader_ID, const char *name, float value);

void SetShaderMat4(int Shader_ID, const char *name, mat4 matrix);

void SetShaderVec2(int Shader_ID, const char *name, vec2 vector);

void SetShaderVec3(int Shader_ID, const char *name, vec3 vector);

void SetShaderVec4(int Shader_ID, const char *name, vec4 value);

void SetShaderVec3v(int Shader_ID, const char *name, vec3 *vector, int amount);
# 2 "SpriteLight/engine_include/SpriteLight.h" 2
# 1 "SpriteLight/engine_include/general.h" 1
       

char *ReadTextFile(char *path);
# 3 "SpriteLight/engine_include/SpriteLight.h" 2
# 1 "SpriteLight/engine_include/main.h" 1




# 1 "SpriteLight/engine_include/texture.h" 1
       

typedef struct Texture
{
    unsigned int ID;
    char *path;
    char *type;
    int width;
    int height;
} Texture;

Texture LoadTexture2D(const char *path, float anisotropy, bool gamma);
# 6 "SpriteLight/engine_include/main.h" 2
# 1 "SpriteLight/engine_include/lights.h" 1
       


int MAX_POINT_LIGHTS = 10;

typedef struct PointLight
{
    vec3 position;
    float padding1;
    vec3 color;
    float padding2;
    vec3 ambient;
    float intensity;
    float radius;
    vec3 padding3;
} PointLight;

typedef struct DirectionalLight
{
    vec3 color;
    vec3 ambient;
    vec3 direction;
} DirectionalLight;

typedef struct SpotLight
{
    vec3 pos;
    vec3 color;
    vec3 ambient;
    vec3 direction;
    float cutoff;
    float outer_cutoff;
    float falloff_mulitplier;
} SpotLight;

int point_light_count;

PointLight *point_lights;
GLuint lights_ubo;
GLuint lights_index = 0;
GLuint lights_binding_point = 0;

void InitLights();

PointLight *CreatePointLight(vec3 position, vec3 color, vec3 ambient, float intensity, float radius);

void UpdateLights();

void FreeLights();
# 7 "SpriteLight/engine_include/main.h" 2
       

SDL_Event window_event;
SDL_Window *window;
SDL_GLContext context;

unsigned int g_buffer, g_position, g_normal, g_albedo, post_process_FBO, post_process_color, rboDepth;
float scene_exposure = 1;
Shader geometry_shader, basic, advanced,
    color_shader, circle_shader, downsample_shader,
    upsample_shader, post_process_shader, sky_shader;

float x1 = 0;
float y_1 = 0;
float x2 = 0;
float y2 = 0;

float sx = 200;
float sy = 100;
float sw = 200;
float sh = 200;

float intersectionX = 0;
float intersectionY = 0;

float quad_vertices[] = {

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

float plane_vertices[] = {

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

    0,
    0,
    0,
    0,
    10,
    0,
};

float cube_vertices[] = {

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
unsigned int quadVBO = 0, quadVAO = 0;
unsigned int lineVBO = 0, lineVAO = 0;
unsigned int cubeVBO = 0, cubeVAO = 0;

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

float frame_time;
unsigned int last_frame = 0;
unsigned int current_frame;

int screen_width = 1920;
int screen_height = 1080;

mat4 projection;

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
Bloom bloom;
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
State state;
Texture cube;
Collider Boxes[10];
float frame_time;

mat4 projection, model, view;

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


void DrawCircle(vec3 position, float radius, vec4 color);

Vector3 Vector3Transform(vec3 v, mat4 mat);

Vector2 GetScreenToWorld2D(Vector2 position);

void OnResize(int new_width, int new_height);
void LightingPass();
void engine_init(char *window_name, int width, int height, int bloom_strength);
# 4 "SpriteLight/engine_include/SpriteLight.h" 2


# 1 "SpriteLight/engine_include/collisions.h" 1

bool CheckCollisionRec(Rectangle rec1, Rectangle rec2);
bool CheckColliders(Collider col1, Collider col2);
Collider GetCollisionArea(Collider rec1, Collider rec2);
bool CheckRotatingColliders(Collider col1, Collider col2);


PointIntersect lineLine(float x1, float y_1, float x2, float y2, float x3, float y3, float x4, float y4);

PointIntersect lineRect(float x1, float y_1, float x2, float y2, float rx, float ry, float rw, float rh);
int CheckCollisionX(Entity *ent, int index);
int CheckCollisionY(Entity *ent, int index);
bool CheckFloor(Entity *ent, int index);
bool ReactToSolid(Entity *ent);
# 7 "SpriteLight/engine_include/SpriteLight.h" 2
