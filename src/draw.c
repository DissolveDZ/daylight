#include "main.h"
// #include "collisions.c"
float p = 0;
void Draw()
{
    p += frame_time;
    glClearColor(0.25f, 0.4f, 0.3f, 1.0f);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
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

    // 3D
    DrawCube((vec3){1, 1.5, 0.5f}, (vec3){1, 1, 1}, (Vector3){p * 20, 0, p * 20});
    DrawCube((vec3){5, 2, 0.5f}, (vec3){5, 1, 1}, (Vector3){40, 40.f, 20.f});
    DrawCube((vec3){Boxes[1].x, Boxes[1].y, 0.5f}, (vec3){Boxes[1].width, Boxes[1].height, 1}, (Vector3){0.f, 0.f, 0.f});
    DrawCube((vec3){Boxes[2].x, Boxes[2].y, 0.5f}, (vec3){Boxes[2].width, Boxes[2].height, 1}, (Vector3){0.f, 0.f, 0.f});

    // UI
    // DrawRectangleBasic((Rectangle){state.mouse_world.x, state.mouse_world.y, 0.25f, 0.25f, 0}, (vec4){255.f, 50.f, 50.f, 255.f});
    // DrawLine((Vector2){state.player.entity.col.x, state.player.entity.col.y}, state.mouse_world, (vec4){50.f, 50.f, 50.f, 255.f});
    // DrawLine((Vector2){state.player.entity.col.x, state.player.entity.col.y}, (Vector2){state.player.entity.col.x + state.player.entity.velocity.x * frame_time * 20, state.player.entity.col.y + state.player.entity.velocity.y * frame_time * 20}, (vec4){255.f, 50.f, 50.f, 255.f});
    // DrawRectangleBasic((Rectangle){intersectionX, intersectionY, 0.25f, 0.25f, 0}, (vec4){0.f, 0.f, 0.f, 255.f});

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    UseShader(advanced);
    SetShaderInt(advanced.ID, "gPosition", 0);
    SetShaderInt(advanced.ID, "gNormal", 1);
    SetShaderInt(advanced.ID, "gAlbedoSpec", 2);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
    SetShaderVec3(advanced.ID, "view_pos", (vec3){state.camera.position.x, state.camera.position.y, state.camera.position.z});
    DrawQuad();

    glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // write to default framebuffer
    // blit to default framebuffer. Note that this may or may not work as the internal formats of both the FBO and default framebuffer have to match.
    // the internal formats are implementation defined. This works on all of my systems, but if it doesn't on yours you'll likely have to write to the
    // depth buffer in another shader stage (or somehow see to match the default framebuffer's internal format with the FBO's internal format).
    glBlitFramebuffer(0, 0, screen_width, screen_height, 0, 0, screen_width, screen_height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // send light relevant uniforms

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