#include "main.h"

void BufferSetup(unsigned int *VAO, unsigned int *VBO, float vertices[], int size, bool textured, bool normals)
{
    glGenVertexArrays(1, VAO);
    glGenBuffers(1, VBO);
    glBindVertexArray(*VAO);
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    if (normals)
    {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }
    else if (textured)
    {
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    }
    else
    {
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    }
}

void Init()
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    window = SDL_CreateWindow("Wedoe Wonder", 0, 0, screen_width, screen_height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_FULLSCREEN);
    context = SDL_GL_CreateContext(window);
    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
    SDL_GL_SetSwapInterval(1);
    // SDL_SetRelativeMouseMode(SDL_TRUE);
    basic = LoadShader("resources/shaders/vertex.vert", "resources/shaders/fragment.frag");
    advanced = LoadShader("resources/shaders/normal.vert", "resources/shaders/fragment.frag");
    color_shader = LoadShader("resources/shaders/color.vert", "resources/shaders/color.frag");
    BufferSetup(&planeVAO, &VBO, plane_vertices, sizeof(plane_vertices), true, false);
    BufferSetup(&lineVAO, &lineVBO, line_vertices, sizeof(line_vertices), false, false);
    BufferSetup(&cubeVAO, &cubeVBO, cube_vertices, sizeof(cube_vertices), true, true);
    stbi_set_flip_vertically_on_load(1);
    // glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);
    state.recs_max = 10;
    state.recs = malloc(sizeof(unsigned int) * state.recs_max);
    state.quit = false;
    state.camera.fov = 45.f;
    state.camera.position = (Vector3){0, 0, 25};
    state.camera.zoom = state.camera.position.z;
    current_frame = SDL_GetPerformanceCounter();
    int spacing = 0;
    for (int i = 0; i < MAX_BUILDINGS; i++)
    {
        buildings[i].width = (float)GetRandomValue(2, 10);
        buildings[i].height = (float)GetRandomValue(5, 40);
        buildings[i].y = buildings[i].height / 2;
        buildings[i].x = -400.0f + buildings[i].width / 2 + spacing;
        spacing += buildings[i].width;
        buildColors[i][0] = GetRandomValue(200, 240);
        buildColors[i][1] = GetRandomValue(200, 240);
        buildColors[i][2] = GetRandomValue(200, 250);
        buildColors[i][3] = 255;
    }
    state.player.entity.speed = 20;
    state.player.entity.accel = 5.0f;
    state.player.entity.decel = 0.05f;
    state.player.entity.velocity = (Vector2){0, 0};
    state.player.entity.max_speed = 15.f;
    state.player.entity.jump_height = 15.5f;
    state.player.entity.health = 100.f;
    state.player.entity.is_floor = false;
    state.player.was_floor = false;
    state.player.turn_mult = 1.5f;
    state.player.last_velocity = (Vector2){0, 0};
    state.player.vertical_to_horizontal = 0.1f;
    state.gravity = -9.81f;
    state.cur_colliders = 0;
    Boxes[0] = RecToCollider((Rectangle){0, -200, 650, 400, 0}, false, false);
    Boxes[1] = RecToCollider((Rectangle){10, 2.5f, 10, 0.25f}, false, false);
    Boxes[2] = RecToCollider((Rectangle){10, 7, 2.5f, 2.5f}, false, false);
   // Boxes[2] = RecToCollider((Rectangle){-100, 225, 200, 200}, false, false);
   // Boxes[3] = RecToCollider((Rectangle){500, 225, 200, 200}, false, false);
    //Boxes[4] = RecToCollider((Rectangle){100, 275, 50, 50}, false, false);
    //Boxes[6] = RecToCollider((Rectangle){800, 225, 200, 5}, true, false);

    InitLights();
    pight = CreatePointLight((vec3){0, 0, 1}, (vec3){2, 2, 2}, (vec3){0.1f, 0.1f, 0.1f}, 1, 1);
    PointLight *leight = CreatePointLight((vec3){5, 2, 2}, (vec3){1, 1, 1}, (vec3){0.1f, 0.1f, 0.1f}, 3.4f, 1.2f);
    PointLight *dr = CreatePointLight((vec3){0, 1, 1}, (vec3){1, 0.7, 0.2}, (vec3){0.1f, 0.1f, 0.1f}, 3.4f, 1.2f);
    PointLight *v = CreatePointLight((vec3){0, 4, 1}, (vec3){1, 0.7, 0.2}, (vec3){0.1f, 0.1f, 0.1f}, 3.4f, 1.2f);
    PointLight *f = CreatePointLight((vec3){0, 8, 1}, (vec3){1, 0.7, 0.2}, (vec3){0.1f, 0.1f, 0.1f}, 3.4f, 1.2f);
    PointLight *s = CreatePointLight((vec3){0, -1, 1}, (vec3){1, 0.7, 0.2}, (vec3){0.1f, 0.1f, 0.1f}, 3.4f, 1.2f);
    PointLight *sie = CreatePointLight((vec3){1, 0, 1}, (vec3){1, 0.7, 0.2}, (vec3){0.1f, 0.1f, 0.1f}, 3.4f, 1.2f);
    PointLight *ach = CreatePointLight((vec3){4, 0, 1}, (vec3){1, 0.7, 0.2}, (vec3){0.1f, 0.1f, 0.1f}, 3.4f, 1.2f);
    PointLight *neu = CreatePointLight((vec3){8, 0, 1}, (vec3){1, 0.7, 0.2}, (vec3){0.1f, 0.1f, 0.1f}, 3.4f, 1.2f);
    PointLight *zeh = CreatePointLight((vec3){-1, 0, 1}, (vec3){1, 0.7, 0.2}, (vec3){0.1f, 0.1f, 0.1f}, 5.0f, 1.2f);
    //point = CreatePointLight((vec3){0, 0, 1}, (vec3){1, 1, 1}, (vec3){0.1f, 0.1f, 0.1f}, 1);
    //printf("Point lights count: %d\n", light_ubo_data->point_light_count);
    // Now update the UBO with the new data
    //UseShader(basic);
    SetShaderInt(basic.ID, "tex", 0);
    state.player.entity.tex = LoadTexture2D("resources/vedl.png", 0, false);
    cube = LoadTexture2D("resources/cube.png", 0, false);
    state.player.entity.col = RecToCollider((Rectangle){0, 20, 2, 2, 0}, false, true);
    state.player.entity.floor_check = (Rectangle){state.player.entity.col.x, state.player.entity.col.y - state.player.entity.col.height / 2, state.player.entity.col.width, 0.05f};
    // state.player.entity.tex.width = state.player.entity.col.width;
    // state.player.entity.tex.height = state.player.entity.col.height;
    OnResize(screen_width, screen_height);
    /*

    water = (Rectangle){1200, 225, 200, 200};
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(screen_width, screen_height, "Wedoe Wonder");
    water_noise = LoadTexture("resources/water_noise.png");
    pight = CreateLight(LIGHT_POINT, (Vector2){0, 255}, Vector2Zero(), WHITE, light_shader);
    water_shader = LoadShader(0, "resources/shaders/water.fs");
    light_shader = LoadShader("resources/shaders/lighting.vs", "resources.shaders/lighting.fs");
    light_shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(light_shader, "viewPos");
    int ambientLoc = GetShaderLocation(light_shader, "ambient");
    SetShaderValue(light_shader, ambientLoc, (float[4]){ 0.1f, 0.1f, 0.1f, 1.0f }, SHADER_UNIFORM_VEC4);
    OnResize();
    state.camera.target = (Vector2){state.player.entity.col.x + 20.0f, state.player.entity.col.y + 20.0f};
    state.camera.rotation = 0.0f;
    state.camera.zoom = 1.0f;
    SetTargetFPS(170);
    */
}