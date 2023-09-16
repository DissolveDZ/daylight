void engine_init(char *window_name, int width, int height, int bloom_strength)
{
    screen_width = width;
    screen_height = height;
    window = SDL_CreateWindow(window_name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_FULLSCREEN_DESKTOP); //
    context = SDL_GL_CreateContext(window);
    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
    SDL_GL_SetSwapInterval(1);
    // SDL_SetRelativeMouseMode(SDL_TRUE);
    stbi_set_flip_vertically_on_load(1);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    BloomInit(bloom_strength);
    GBufferSetup();
    PostProcessBuffer();

    // make LoadShader() relative to shader path
    geometry_shader = LoadShader("resources/shaders/buffers.vert", "resources/shaders/buffers.frag");
        printf("basicc shader: %i\n", geometry_shader.ID);
    basic = LoadShader("resources/shaders/buffers.vert", "resources/shaders/buffers.frag");

    advanced = LoadShader("resources/shaders/vertex.vert", "resources/shaders/fragment.frag");
    color_shader = LoadShader("resources/shaders/buffers.vert", "resources/shaders/color.frag");
    circle_shader = LoadShader("resources/shaders/buffers.vert", "resources/shaders/circle.frag");
    post_process_shader = LoadShader("resources/shaders/vertex.vert", "resources/shaders/post_process.frag");
    sky_shader = LoadShader("resources/shaders/vertex.vert", "resources/shaders/sky.frag");

    UseShader(post_process_shader);
    SetShaderInt(post_process_shader.ID, "lighting", 0);
    SetShaderInt(post_process_shader.ID, "bloom", 1);

    UseShader(advanced);
    SetShaderInt(advanced.ID, "g_position", 0);
    SetShaderInt(advanced.ID, "g_normal", 1);
    SetShaderInt(advanced.ID, "g_albedo", 2);

    BufferSetup(&planeVAO, &VBO, plane_vertices, sizeof(plane_vertices), true, false);
    BufferSetup(&quadVAO, &quadVBO, quad_vertices, sizeof(quad_vertices), true, false);
    BufferSetup(&lineVAO, &lineVBO, line_vertices, sizeof(line_vertices), false, false);
    BufferSetup(&cubeVAO, &cubeVBO, cube_vertices, sizeof(cube_vertices), true, true);
}