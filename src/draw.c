#include "main.h"
// #include "collisions.c"
void Draw()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    vec3 temp;
    glm_mat4_identity(view);
    vec3 camera_pos = {state.camera.position.x, state.camera.position.y, state.camera.position.z};
    glm_vec3_add(camera_pos, (vec3){0, 0, -1}, temp);
    glm_lookat(camera_pos, temp, (vec3){0, 1, 0}, view);
    for (int i = 0; i < MAX_BUILDINGS; i++)
    {
        DrawRectangleBasic(buildings[i], buildColors[i]);
    }
    for (int i = 0; i < state.cur_colliders; i++)
    {
        DrawRectangleBasic((Rectangle){Boxes[i].x, Boxes[i].y, Boxes[i].width, Boxes[i].height, 0}, (vec4){125.f, 125.f, 125.f, 255.f});
    }
    DrawEntity((Rectangle){state.player.entity.col.x, state.player.entity.col.y, state.player.entity.col.width, state.player.entity.col.height, state.player.entity.tex.ID});
    DrawRectangleBasic((Rectangle){state.mouse_world.x, state.mouse_world.y, 0.25f, 0.25f, 0}, (vec4){255.f, 50.f, 50.f, 255.f});
    DrawLine((Vector2){state.player.entity.col.x, state.player.entity.col.y}, state.mouse_world, (vec4){50.f, 50.f, 50.f, 255.f});
    DrawLine((Vector2){state.player.entity.col.x, state.player.entity.col.y}, (Vector2){state.player.entity.col.x + state.player.entity.velocity.x * frame_time * 20, state.player.entity.col.y + state.player.entity.velocity.y * frame_time * 20}, (vec4){255.f, 50.f, 50.f, 255.f});
    DrawRectangleBasic((Rectangle){intersectionX, intersectionY, 0.25f, 0.25f, 0}, (vec4){0.f, 0.f, 0.f, 255.f});
    DrawCube((vec3){1, 2, 0.5f}, (vec3){1, 1, 1}, (vec3){0.5f, 0.5f, 0.5f});
    DrawCube((vec3){3, 2, 0.5f}, (vec3){5, 1, 1}, (vec3){-0.25f, -0.25f, -0.25f});
    SDL_GL_SwapWindow(window);
    /*
    DrawTextureV(water_noise, (Vector2){water.x, water.y}, WHITE);
    EndShaderMode();
    // RotateRec(&testing, rotation, (Vector2){0, 0});
    // DrawTrueRectangleTex(testing, state.player.entity.tex, GRAY);
    //printf("x: %f y: %f\n", testing.pos.x, testing.pos.y);
    // DrawRectangleRec(state.player.entity.col, BLACK);
    DrawText(TextFormat("yuy x: %f, y: %f", state.player.entity.col.x, state.player.entity.col.y), 0, 0, 20, RED);
    drawcol();
    // DrawRectangleRec(state.player.entity.floor_check, BLACK);
    EndMode2D();
    DrawText(TextFormat("yuy:\n pos x: %f, y: %f\n vel x: %f y: %f\n speed: %f\n acceleration: %f\n deceleration: %f\n next position: %f\n maximum/minimum speed: %f\n jump height: %f\n health: %f\n turn multiplier: %f\n on_floor: %b\n", state.player.entity.col.x, state.player.entity.col.y, state.player.entity.velocity.x, state.player.entity.velocity.y, state.player.entity.speed, state.player.entity.accel, state.player.entity.decel, state.player.entity.col.x + state.player.entity.velocity.x, state.player.entity.max_speed, state.player.entity.jump_height, state.player.entity.health, state.player.turn_mult, state.player.entity.is_floor), 0, 0, 30, RED);
    EndDrawing();
    */
}