void init()
{
    max_weeds = grid_x * grid_y;
    InitAudioDevice();
    InitWindow(window_width, window_height, Game_Name);
    rander = GetRandomValue(0, 5);
    switch (rander)
    {
    case 0:
        Game_Name = "g4ming (weed)";
        break;
    case 1:
        Game_Name = "generic weed game";
        break;
    case 2:
        Game_Name = "don't TOUCH the grass";
        break;
    case 3:
        Game_Name = "i think they're onto us Guenther!!";
        break;
    case 4:
        Game_Name = "shake the baby 0)_(o";
        break;
    case 5:
        Game_Name = "who knows how long he has been sitting there";
        break;
    }
    SetWindowTitle(Game_Name);
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    cursor1 = LoadTexture("resources/textures/cursor1.png");
    cursor2 = LoadTexture("resources/textures/cursor2.png");
    dirt_texture = LoadTexture("resources/textures/dirt.png");
    seed_bag_weed1 = LoadTexture("resources/textures/seed_bag_weed1.png");
    weed_planted = LoadTexture("resources/textures/weed_planted.png");
    weed_sapling = LoadTexture("resources/textures/weed_sapling.png");
    weed_grow1 = LoadTexture("resources/textures/weed_grow1.png");
    weed_grow2 = LoadTexture("resources/textures/weed_grow2.png");
    weed_grow3 = LoadTexture("resources/textures/weed_grow3.png");
    weed_texture = LoadTexture("resources/textures/weed.png");
    weed_planted.width = 64;
    weed_planted.height = 64;
    weed_sapling.width = 64;
    weed_sapling.height = 64;
    weed_grow1.width = 64;
    weed_grow1.height = 64;
    weed_grow2.width = 64;
    weed_grow2.height = 64;
    weed_grow3.width = 64;
    weed_grow3.height = 64;
    weed_texture.width = 64;
    weed_texture.height = 64;
    cursor1.width = 64;
    cursor1.height = 64;
    cursor2.width = 64;
    cursor2.height = 64;
    cursor_texture.width = 64;
    cursor_texture.height = 64;
    dirt_texture.width = 64;
    dirt_texture.height = 64;
    seed_bag_weed1.width = 64;
    seed_bag_weed1.height = 64;
    plant_sound = LoadSound("resources/sounds/plant.mp3");
    pixelfont = LoadFont("resources/fonts/Pixel_Square.ttf");
    plant_message_array = malloc(max_plant_messages_on_screen * sizeof(Text));
    weed_instance = LoadShader("resources/shaders/weed_instanced.vs", NULL);
    plant_message_displacement = LoadShader(0, "resources/shaders/warp.fs");
    weed_model = LoadModel("resources/models/plant.obj");
    weed_material = LoadMaterialDefault();
    weed_material.shader = weed_instance;
    music = LoadMusicStream("resources/sounds/35_Return_Trip.mp3");
    PlayMusicStream(music);
    seed = LoadTexture("resources/textures/seed.png");
    dig_sound = LoadSound("resources/sounds/dig.mp3");
    seed.width = 128;
    seed.height = 64;
    upgrades_rec_color = (Color){127, 106, 79, 125};
    Upgrade_Menu = CLOSED;

    SetWindowState(FLAG_WINDOW_MAXIMIZED);
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    SetTargetFPS(144);
    weed_array = malloc(grid_x * sizeof(void *));

    for (int o = 0; o < grid_x; o++)
    {
        weed_array[o] = malloc(grid_y * sizeof(Weed));
        for (int i = 0; i < grid_y; i++)
        {
            weed_array[o][i].type = DIRT;
            weed_array[o][i].planted = false;
            weed_array[o][i].time = 0;
            weed_array[o][i].matrix = MatrixIdentity();
        }
    }
    Resize();
    UpdatePlants();
}