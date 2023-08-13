inline Vector2I GetScreenToWorld2DVectorI(Vector2 position, Camera2D camera)
{
    Vector2 temp = GetScreenToWorld2D(position, camera);
    return (Vector2I){temp.x, temp.y};
}
bool CheckOutOfBounds(Vector2I position, directions direction)
{
    switch (direction)
    {
    case RIGHT:
        if (position.x < grid_x - 1)
            return true;
        else
            return false;
    case LEFT:
        if (position.x > 0)
            return true;
        else
            return false;
    case UP:
        if (position.y > 0)
            return true;
        else
            return false;
    case DOWN:
        if (position.y < grid_y - 1)
            return true;
        else
            return false;
    }
}
Plant(Vector2I position)
{
    if (!weed_array[position.x][position.y].planted)
    {
        if (seeds > 0)
        {
            weed_array[position.x][position.y].planted = true;
            seeds -= 1;
            UpdatePlants();
            PlaySoundMulti(plant_sound);
            if (GetRandomValue(1, 20) == 10)
            {
                cur_plant_messages_on_screen++;
                if (cur_plant_messages_on_screen == max_plant_messages_on_screen - 1)
                {
                    max_plant_messages += 10;
                    plant_message_array = realloc(plant_message_array, sizeof(Text) * max_plant_messages);
                }
                plant_message_array[cur_plant_messages_on_screen - 1].position = ((Vector2){GetRandomValue(0, window_width), GetRandomValue(0, window_height)});
                plant_message_array[cur_plant_messages_on_screen - 1].random = GetRandomValue(0, 9);
                plant_message_array[cur_plant_messages_on_screen - 1].velocity.y = GetRandomValue(1, 50);
                plant_message_array[cur_plant_messages_on_screen - 1].size = GetRandomValue(20, 60);
                plant_message_array[cur_plant_messages_on_screen - 1].lifetime = 0;
                plant_message_array[cur_plant_messages_on_screen - 1].opacity = 255;
            }
        }
    }
    else
    {
        for (int i = 0; i < max_weeds; i++)
            if (weed_array[position.x][position.y].type == GROWN)
            {
                weed_array[position.x][position.y].planted = false;
                PlaySoundMulti(dig_sound);
                UpdatePlants();
                weeds++;
            }
    }
}
void update()
{
    mouse_pos = GetMousePosition();
    switch (Upgrade_Menu)
    {
    case CLOSED:
        frame_time = GetFrameTime();
        time_passed += frame_time;
        time_sassed += frame_time * 7;
        time_cassed += frame_time;
        if (time_cassed >= .1f)
        {
            time_cassed -= .1f;
            for (int i = 0; i < grid_x; i++)
            {
                for (int o = 0; o < grid_y; o++)
                    weed_array[i][o].time++;
            }
            UpdatePlants();
        }
        if (IsWindowResized())
        {
            window_width = GetScreenWidth();
            window_height = GetScreenHeight();
            Resize();
            UpdatePlants();
        }
        if ((int)time_sassed % 2)
        {
            toggle_cursor = !toggle_cursor;
            time_sassed = 0;
        }
        if (toggle_cursor)
            cursor_texture = cursor1;
        else
            cursor_texture = cursor2;

        if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT))
        {
            direction = RIGHT;
            if (CheckOutOfBounds(cursor_pos, direction))
            {
                cursor_pos.x += 1;
            }
        }
        if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT))
        {
            direction = LEFT;
            if (CheckOutOfBounds(cursor_pos, direction))
            {
                cursor_pos.x -= 1;
            }
        }
        if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP))
        {
            direction = UP;
            if (CheckOutOfBounds(cursor_pos, direction))
            {
                cursor_pos.y -= 1;
            }
        }
        if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN))
        {
            direction = DOWN;
            if (CheckOutOfBounds(cursor_pos, direction))
            {
                cursor_pos.y += 1;
            }
        }
        if (IsKeyPressed(KEY_F11))
            ToggleFullscreen();
        if (IsKeyPressed(KEY_ENTER))
        {
            Plant(cursor_pos);
        }
        if (camera.offset.x + window_width < 0)
            printf("display plaent");
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && !hover_ui)
        {
            Vector2 delta = GetMouseDelta();
            delta = Vector2Scale(delta, -1.0f / camera.zoom);

            camera.target = Vector2Add(camera.target, delta);
        }
        float wheel = GetMouseWheelMove();
        if (wheel != 0)
        {
            Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
            camera.offset = GetMousePosition();
            camera.target = mouseWorldPos;
            const float zoomIncrement = 0.125f;
            camera.zoom += (wheel * zoomIncrement);
            if (camera.zoom < zoomIncrement)
                camera.zoom = zoomIncrement;
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !hover_ui)
        {
            Vector2I temp = GetScreenToWorld2DVectorI(mouse_pos, camera);
            Vector2I rounded_click = (Vector2I){floorf(temp.x), floorf(temp.y)};
            cursor_pos.x = rounded_click.x;
            cursor_pos.y = rounded_click.y;
            if (rounded_click.x < 0)
                cursor_pos.x = 0;
            if (rounded_click.x > grid_x - 1)
                cursor_pos.x = grid_x - 1;
            if (rounded_click.y < 0)
                cursor_pos.y = 0;
            if (rounded_click.y > grid_y - 1)
                cursor_pos.y = grid_y - 1;
            if (last_cursor_pos.x == cursor_pos.x && last_cursor_pos.y == cursor_pos.y)
                Plant(cursor_pos);
            last_cursor_pos = cursor_pos;
        }
        break;
    }
    if (CheckCollisionPointRec(mouse_pos, upgrades_rec) || IsKeyPressed(KEY_TAB))
    {
        hover_ui = true;
        upgrades_rec_color = (Color){127 - 50, 106 - 50, 79 - 50, 125};
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsKeyPressed(KEY_TAB))
        {
            if (Upgrade_Menu == CLOSED)
                Upgrade_Menu = SKILL;
            else if (Upgrade_Menu == SKILL)
                Upgrade_Menu = CLOSED;
        }
    }
    else
    {
        upgrades_rec_color = (Color){127, 106, 79, 125};
        if (Upgrade_Menu == CLOSED)
            hover_ui = false;
    }
    UpdateMusicStream(music);
}