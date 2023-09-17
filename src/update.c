static inline void PlayerMovement(Player *player)
{
    uint8_t *key_state = SDL_GetKeyboardState(NULL);
    int dir = 0;
    if (key_state[SDL_SCANCODE_A] && !key_state[SDL_SCANCODE_D])
    {
        dir = -1;
        player->entity.flip = false;
        // player->entity.col.width = player->entity.tex.width;
    }
    if (key_state[SDL_SCANCODE_D] && !key_state[SDL_SCANCODE_A])
    {
        dir = 1;
        player->entity.flip = true;
        // player->entity.col.width = -player->entity.tex.width;
    }
    player->entity.is_floor = false;
    Rectangle *pos = &player->entity.col;
    float desired_speed = 0;
    // printf("velocity x: %f velocity y: %f\n", player->entity.velocity.x, player->entity.velocity.y);
    if (key_state[SDL_SCANCODE_A] || key_state[SDL_SCANCODE_D] && dir != 0)
    {
        player->state = WALKING;
        desired_speed += player->entity.speed * frame_time;
        if (player->entity.velocity.x + (desired_speed * dir) > player->entity.max_speed || player->entity.velocity.x + (desired_speed * dir) < -player->entity.max_speed)
        {
            // player->entity.velocity.x = player->entity.max_speed * dir;
            if (dir > 0 && player->entity.velocity.x < 0 || dir < 0 && player->entity.velocity.x > 0)
            {
                player->entity.velocity.x += desired_speed * dir;
            }
        }
        else
        {
            if (dir > 0 && player->entity.velocity.x < 0 || dir < 0 && player->entity.velocity.x > 0)
            {
                player->entity.velocity.x += desired_speed * dir * player->turn_mult;
            }
            else
                player->entity.velocity.x += desired_speed * dir;
        }
    }
    if (player->state == WALKING)
    {
    }
    for (int i = 0; i < state.cur_colliders; i++)
    {
        CheckFloor(&player->entity, i);
    }
    if (dir == 0 && player->entity.is_floor)
    {
        player->entity.velocity.x = Lerp(player->entity.velocity.x, 0, player->entity.accel * frame_time); // * 1 - clamp(player->entity.velocity.x)
    }
    else if (player->entity.velocity.x > player->entity.max_speed || player->entity.velocity.x < -player->entity.max_speed)
    {
        player->entity.velocity.x = Lerp(player->entity.velocity.x, 0, player->entity.accel * player->entity.decel * frame_time);
    }
    if (!player->entity.is_floor)
        player->entity.velocity.y += state.gravity * 4 * frame_time;
    float speed_given = 0;
    /*
    if (player->was_floor != player->entity.is_floor)
    {
        player->was_floor = player->entity.is_floor;
        if (player->was_floor)
        {
            //printf("velocity x: %f\n", player->last_velocity.x);
            //printf("velocity y: %f\n", player->last_velocity.y);
            float angle = RAD2DEG * Vector2Angle((Vector2){0, player->last_velocity.y}, (Vector2){player->entity.velocity.x, 0}) + 90.f;
            // float fract = RAD2DEG * Vector2Angle((Vector2){0, 1}, (Vector2){1, 0});
            //  state.player.entity.velocity.y -= fract;
            //printf("fract: %f\n", angle);
            speed_given = player->vertical_to_horizontal * angle;
            //printf("%f\n", speed_given);
        }
    }
    if (speed_given > 1)
        speed_given = 1;
    else if (speed_given < -1)
        speed_given = -1;
    */
    if (key_state[SDL_SCANCODE_SPACE] && player->entity.is_floor)
    {
        player->entity.velocity.y = player->entity.jump_height;
    }
    player->last_velocity = player->entity.velocity;
    bool col = ReactToSolid(&player->entity);
    // printf("col: %d\n", col);
    player->entity.col.y += player->entity.velocity.y * frame_time;
    player->entity.col.x += player->entity.velocity.x * frame_time;
    player->entity.floor_check.x = player->entity.col.x;
    player->entity.floor_check.y = player->entity.col.y - player->entity.col.height / 2;
    /*
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
    {
        player->entity.col.x = state.mouse_world.x;
        player->entity.col.y = state.mouse_world.y;
    }
    */
}
void ProcessInputs()
{
    while (SDL_PollEvent(&window_event))
    {
        // if (window_event.type == SDL_QUIT || window_event.key.keysym.sym == SDLK_ESCAPE)
        // state.quit = true;
        // if (window_event.key.keysym.sym == SDLK_LEFT) vedoe.width = 1;
        // if (window_event.key.keysym.sym == SDLK_RIGHT) vedoe.width = -1;
        /*
        switch(window_event.key.keysym.sym)
        {
            case SDLK_LEFT:
                vedoe.width = 1;
                break;
            case SDLK_RIGHT:
                vedoe.width = -1;
                break;
        }
        */
        switch (window_event.type)
        {
        case SDL_QUIT:
            state.quit = true;
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (window_event.button.button == SDL_BUTTON_RIGHT)
            {
                state.player.entity.col.x = state.mouse_world.x;
                state.player.entity.col.y = state.mouse_world.y;
            }
            break;
        case SDL_MOUSEWHEEL:
            if (window_event.wheel.y > 0)
            {
                if (state.camera.zoom > 1.1)
                    state.camera.zoom -= 2;
            }
            else if (window_event.wheel.y < 0)
            {
                state.camera.zoom += 2;
            }
            break;
        case SDL_KEYDOWN:
            switch (window_event.key.keysym.scancode)
            case SDL_SCANCODE_F11:
                state.fullscreen = !state.fullscreen;
            if (state.fullscreen)
                SDL_SetWindowFullscreen(main_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
            else
                SDL_SetWindowFullscreen(main_window, SDL_WINDOW_BORDERLESS);
            // SDL_SetWindowDisplayMode(window, SDL_WINDOW_BORDERLESS);
            break;
            break;
        case SDL_WINDOWEVENT:
            if (window_event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
            {
                // resize(window_event.window.data1, window_event.window.data2);
                if (window_event.window.data1 || window_event.window.data2)
                    OnResize(window_event.window.data1, window_event.window.data2);
            }
            break;
        default:
            break;
        }
    }
}
void Update()
{
    ProcessInputs();
    last_frame = current_frame;
    current_frame = SDL_GetPerformanceCounter();
    frame_time = (double)(current_frame - last_frame) / (double)SDL_GetPerformanceFrequency();
    PlayerMovement(&state.player);
    glm_mat4_identity(projection);
    if (screen_height && screen_width)
        glm_perspective(glm_rad((float)screen_height / state.camera.fov), (float)screen_width / (float)screen_height, 0.1, 1000, projection);
    printf("fov: %f\n", (float)screen_height);
    state.camera.position.x = Lerp(state.camera.position.x, state.player.entity.col.x, 7.5f * frame_time);
    state.camera.position.y = Lerp(state.camera.position.y, state.player.entity.col.y, 7.5f * frame_time);
    state.camera.position.z = Lerp(state.camera.position.z, state.camera.zoom, 7.5f * frame_time);
    int x, y;
    SDL_GetMouseState(&x, &y);
    // printf("x: %i y: %i\n", x, y);
    state.mouse_world = GetScreenToWorld2D((Vector2){x, y}, projection);
    pight->position[0] = state.player.entity.col.x;
    pight->position[1] = state.player.entity.col.y;
    pight->position[2] = 1.0f;
    UpdateLights();
    // printf("mouse x: %f mouse y: %f\n", state.mouse_world.x, state.mouse_world.y);
    // printf("framerate: %f\n", 1/frame_time);
    /*
UpdateLightValues(light_shader, pight);
state.player.state = IDLE;
frame_time = GetFrameTime();
// Camera target follows player
state.camera.target = (Vector2){state.player.entity.col.x + 20, state.player.entity.col.y + 20};

// Limit camera rotation to 80 degrees (-40 to 40)
if (state.camera.rotation > 40)
    state.camera.rotation = 40;
else if (state.camera.rotation < -40)
    state.camera.rotation = -40;

// Camera zoom controls
state.camera.zoom += ((float)GetMouseWheelMove() * 0.05f);

if (state.camera.zoom > 10.0f)
    state.camera.zoom = 10.0f;
else if (state.camera.zoom < 0.01f)
    state.camera.zoom = 0.01f;
// Update the shader with the camera view vector (points towards { 0.0f, 0.0f, 0.0f })
float cameraPos[3] = {state.camera.target.x, state.camera.target.y, 0};
SetShaderValue(light_shader, light_shader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);
// Camera reset (zoom and rotation)
if (IsKeyPressed(KEY_R))
{
    state.camera.zoom = 1.0f;
    state.camera.rotation = 0.0f;
}
*/
}