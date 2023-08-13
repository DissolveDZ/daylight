void draw()
{
    if (cur_plant_messages_on_screen > 0)
    {
        BeginTextureMode(plant_messages_rendertexture);
        ClearBackground((Color){0, 0, 0, 0});
        for (size_t i = 0; i < cur_plant_messages_on_screen; i++)
        {
            plant_message_array[i].lifetime += frame_time * 6;
            DrawText(TextSubtext(plant_messages[plant_message_array[i].random], 0, (int)plant_message_array[i].lifetime), plant_message_array[i].position.x, plant_message_array[i].position.y, plant_message_array[i].size, (Color){255, 33, 0, plant_message_array[i].opacity});
            plant_message_array[i].position.y += plant_message_array[i].velocity.y * frame_time;
            plant_message_array[i].opacity = Lerp(plant_message_array[i].opacity, 0, .2 * frame_time);
            if (plant_message_array[i].opacity < 10)
            {
                for (int o = i; o < cur_plant_messages_on_screen; o++)
                {
                    plant_message_array[o] = plant_message_array[o + 1];
                }
                cur_plant_messages_on_screen -= 1;
            }
        }
        DrawRectangleRec((Rectangle){0, 0, (float)plant_messages_rendertexture.texture.width, (float)-plant_messages_rendertexture.texture.height}, WHITE);
        EndTextureMode();
    }
    BeginDrawing();
    ClearBackground(GREEN);
    DrawTextureRec(background_texture, (Rectangle){0, 0, window_width, window_height}, (Vector2){0, 0}, WHITE);
    BeginMode3D(camera);
    DrawMeshInstanced(weed_model.meshes[0], weed_material);
    DrawTextureV(cursor_texture, (Vector2){cursor_pos.x, cursor_pos.y}, WHITE);
    EndMode3D();
    if (cur_plant_messages_on_screen > 0)
    {
        BeginShaderMode(plant_message_displacement);
        DrawTextureRec(plant_messages_rendertexture.texture, (Rectangle){0, 0, (float)plant_messages_rendertexture.texture.width, (float)-plant_messages_rendertexture.texture.height}, (Vector2){0, 0}, WHITE);
        EndShaderMode();
    }
    DrawTexture(seed, window_width - 500, window_height * .035 - seed.height / 4, WHITE);
    DrawTexture(weed_texture, 200, window_height * .035 - weed_texture.height / 4, WHITE);
    DrawTextureV(seed_bag_weed1, (Vector2){0, 0}, WHITE);
    if (Upgrade_Menu != CLOSED)
    {
        DrawRectangleRec((Rectangle){0, 0, window_width, window_height}, (Color){0, 0, 0, 125});
        DrawRectangleRec((Rectangle){100, 100, window_width - 200, window_height - 200}, (Color){65, 65, 65, 125});
    }
    DrawText(TextFormat("%i: ", weeds), 200 - MeasureText(TextFormat("%i: ", weeds), 50), window_height * .035, 50, BLACK);
    DrawRectangleRec(upgrades_rec, upgrades_rec_color);
    DrawText(TextFormat("%i: ", seeds), window_width - 550 - MeasureText(TextFormat("%i: ", seeds), 50) / 2, window_height * .035, 50, BLACK);
    DrawTextEx(pixelfont, "day:", (Vector2){window_width / 2 - GetTextMiddle("day:", 40, pixelfont, 3) + shop_slide_offset.x, 20}, 40, 3, BLACK);
    DrawTextEx(pixelfont, "upgrades", (Vector2){window_width - 325 + shop_slide_offset.x, window_height * .035}, 50, 3, BLACK);
    DrawFPS(10, 10);
    EndDrawing();
}