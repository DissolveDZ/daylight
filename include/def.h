int window_width;
int window_height;

typedef struct Player
{
    Rectangle rec;
    Texture2D texture;
    __int8 size;
    float walk_speed;
    Vector2 acceleration;
    Vector2 velocity;
} Player;

Player player;

typedef struct Vector2I
{
    int x;
    int y;
} Vector2I;

typedef enum directions
{
    RIGHT,
    LEFT,
    UP,
    DOWN
} directions;
directions direction;

typedef enum upgrade_menu
{
    CLOSED,
    SKILL,
    SEEDS,
    FARMERS,
} upgrade_menu;
upgrade_menu Upgrade_Menu;

typedef enum Weed_Types
{
    DIRT,
    PLANTED,
    SAPLING,
    GROW1,
    GROW2,
    GROW3,
    GROWN,
    DED
} Weed_Types;

typedef struct Weed
{
    Weed_Types type;
    Material mat;
    Vector2I pos;
    Matrix matrix;
    bool watered;
    bool planted;
    float time;
} Weed;

typedef struct Text
{
    char *text;
    Vector2 position;
    Vector2 velocity;
    float size;
    int random;
    float opacity;
    float lifetime;
} Text;

inline DrawTextureVI(Texture2D texture, Vector2I pos, Color tint)
{
    DrawTexture(texture, pos.x, pos.y, tint);
}

Shader plant_message_displacement;
Shader weed_instance;
RenderTexture2D plant_messages_rendertexture;

int window_width = 1920;
int window_height = 1080;
int rander = 0;
char *Game_Name = "";
int max_plant_messages = 10;
int max_plant_messages_on_screen = 10;
int cur_plant_messages_on_screen = 0;
bool hover_ui = false;
char *plant_messages[] =
    {
        "balls planted",
        "mission completed return to HQ",
        "Ich fuehle mich nicht so gut O)v(o",
        "oh Baby a triple!",
        "yuy!",
        "endlich",
        "toll gemacht!?!?",
        "ich frage mich was PAY davon denkt",
        "oh Baby a triple!",
        "nur noch ein bisschen",
};
Text *plant_message_array;
Rectangle upgrades_rec;
Color upgrades_rec_color;
Material weed_material;

Texture2D cursor1;
Texture2D cursor2;
Texture2D cursor_texture;
Texture2D dirt_texture;
Texture2D weed_planted;
Texture2D weed_sapling;
Texture2D weed_grow1;
Texture2D weed_grow2;
Texture2D weed_grow3;
Texture2D weed_texture;
Texture2D background_texture;
Texture2D seed_bag_weed1;

Music music;
Model weed_model;

int grid_x = 1000;
int grid_y = 1000;
int max_weeds;
int seeds = 1000;
int weeds = 0;
Camera camera;

Weed **weed_array;

Sound plant_sound;
Sound dig_sound;
Vector2 shop_slide_offset;
Vector2 mouse_pos;
Vector2I cursor_pos = {0, 0};
Vector2I last_cursor_pos;
float frame_time;
float time_passed;
float time_sassed;
float time_cassed;
float time_hassed;
bool toggle_cursor = false;
Texture2D seed;

Font pixelfont;

Resize()
{
    upgrades_rec = (Rectangle){window_width - 337 + shop_slide_offset.x, window_height * .025, 325, 65};
    Image VerticalGradient = GenImageGradientV(window_width, window_height, (Color){150, 200, 255, 255}, (Color){0, 121 - 50, 241 - 100, 255});
    background_texture = LoadTextureFromImage(VerticalGradient);
    plant_messages_rendertexture = LoadRenderTexture(window_width, window_height);
    float screenSize[2] = {(float)window_width, (float)window_height};
    if (window_width != 0 && window_height != 0)
        SetShaderValue(plant_message_displacement, GetShaderLocation(plant_message_displacement, "size"), &screenSize, SHADER_UNIFORM_VEC2);
    camera.target = (Vector2){grid_x * 64 / 2, grid_y * 64 / 2};
    camera.position = (Vector2){window_width / 2, window_height / 2};
}
UpdatePlants()
{
    for (size_t i = 0; i < grid_x; i++)
    {
        for (size_t o = 0; o < grid_y; o++)
        {
            if (!weed_array[i][o].planted)
            {

                weed_array[i][o].pos = (Vector2I){i, o};
                weed_array[i][o].type = DIRT;
                weed_array[i][o].time = 0;
                weed_array[i][o].planted = false;
            }
            if (weed_array[i][o].planted)
            {
                weed_array[i][o].pos = (Vector2I){i, o};
                if (weed_array[i][o].time <= 10)
                    weed_array[i][o].type = PLANTED;
                else if (weed_array[i][o].time <= 20)
                    weed_array[i][o].type = SAPLING;
                else if (weed_array[i][o].time <= 30)
                    weed_array[i][o].type = GROW1;
                else if (weed_array[i][o].time <= 40)
                    weed_array[i][o].type = GROW2;
                else if (weed_array[i][o].time <= 50)
                    weed_array[i][o].type = GROW3;
                else if (weed_array[i][o].time > 50)
                    weed_array[i][o].type = GROWN;
            }
            switch (weed_array[i][o].type)
            {
            case DIRT:
                weed_array[i][o].mat.maps[MATERIAL_MAP_DIFFUSE].texture = dirt_texture;
                break;
            case PLANTED:
                weed_array[i][o].mat.maps[MATERIAL_MAP_DIFFUSE].texture = weed_planted;
                break;
            case SAPLING:
                weed_array[i][o].mat.maps[MATERIAL_MAP_DIFFUSE].texture = weed_sapling;
                break;
            case GROW1:
                weed_array[i][o].mat.maps[MATERIAL_MAP_DIFFUSE].texture = weed_grow1;
                break;
            case GROW2:
                weed_array[i][o].mat.maps[MATERIAL_MAP_DIFFUSE].texture = weed_grow2;
                break;
            case GROW3:
                weed_array[i][o].mat.maps[MATERIAL_MAP_DIFFUSE].texture = weed_grow3;
                break;
            case GROWN:
                weed_array[i][o].mat.maps[MATERIAL_MAP_DIFFUSE].texture = weed_texture;
                break;
            }
        }
    }
}

vectorDotProduct(Vector2 pt1, Vector2 pt2)
{
    return (pt1.x * pt2.x) + (pt1.y * pt2.y);
}

float GetTextMiddle(char *text, int size, Font font, float spacing)
{
    return MeasureTextEx(font, text, size, spacing).x / 2;
}

Vector2 GetRecVerts(Rectangle rec, int index)
{
    switch (index)
    {
    case 0:
        return (Vector2){rec.x, rec.y};
        break;
    case 1:
        return (Vector2){rec.x + rec.width, rec.y};
        break;
    case 2:
        return (Vector2){rec.x + rec.width, rec.y + rec.height};
        break;
    case 3:
        return (Vector2){rec.x, rec.y + rec.height};
        break;
    }
}