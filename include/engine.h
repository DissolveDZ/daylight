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
SDL_Surface *LoadSDLImage(char *path);
# 6 "SpriteLight/engine_include/main.h" 2
# 1 "SpriteLight/engine_include/lights.h" 1
       


static int MAX_POINT_LIGHTS = 10;

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

static int point_light_count;

static PointLight *point_lights;
static GLuint lights_ubo;
static GLuint lights_index = 0;
static GLuint lights_binding_point = 0;

void InitLights();

PointLight *CreatePointLight(vec3 position, vec3 color, vec3 ambient, float intensity, float radius);

void UpdateLights();

void FreeLights();
# 7 "SpriteLight/engine_include/main.h" 2
       

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

int GetRandomValue(int min, int max);

float Lerp(float start, float end, float amount);

Vector3 Vector3Transform(vec3 v, mat4 mat);

Vector2 GetScreenToWorld2D(Vector2 position, mat4 projection);
void GBufferSetup(unsigned int *g_buffer, unsigned int *g_position, unsigned int *g_normal, unsigned int *g_albedo, unsigned int *depth, int screen_width, int screen_height);
void PostProcessBuffer(unsigned int *post_process_fbo, unsigned int *post_process_color, unsigned int *depth, int screen_width, int screen_height);
void BufferSetup(unsigned int *VAO, unsigned int *VBO, float vertices[], int size, bool textured, bool normals);
void OnResize(int new_width, int new_height);
void LightingPass();
void engine_init(char *window_name, int width, int height, int bloom_strength);
void BloomInit(int mip_amount, Bloom *bloom, int screen_width, int screen_height);
void UpsampleBloom(float filter_radius, Bloom *bloom, unsigned int *quadVAO);
void DownSampleBloom(unsigned int src_texture, float threshold, float knee, Bloom *bloom, unsigned int *quadVAO, int screen_width, int screen_height);
void RenderBloom(unsigned int src_texture, float filter_radius, float threshold, float knee, Bloom *bloom, unsigned int *quadVAO, int screen_width, int screen_height);
# 4 "SpriteLight/engine_include/SpriteLight.h" 2
