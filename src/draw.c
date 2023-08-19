#include "main.h"
// #include "collisions.c"
float p = 0;
float bloom_filter_radius = 0.004f;
void Draw()
{
    p += frame_time;
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glBindFramebuffer(GL_FRAMEBUFFER, g_buffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    vec3 temp;
    glm_mat4_identity(view);
    vec3 camera_pos = {state.camera.position.x, state.camera.position.y, state.camera.position.z};
    glm_vec3_add(camera_pos, (vec3){0, 0, -1}, temp);
    glm_lookat(camera_pos, temp, (vec3){0, 1, 0}, view);

    // Sun
    DrawCircle((vec3){state.camera.position.x - 1.5f, state.camera.position.y + 1.f, state.camera.position.z - 5}, 0.5f, (vec4){255.f * 10, 255.f * 10, 255.f * 10, 255.f});

    for (int i = 0; i < MAX_BUILDINGS; i++)
    {
        DrawRectangleBasic(buildings[i], buildColors[i]);
    }
    for (int i = 0; i < state.cur_colliders; i++)
    {
        DrawRectangleBasic((Rectangle){Boxes[i].x, Boxes[i].y, Boxes[i].width, Boxes[i].height, 0}, (vec4){50.f, 180.f, 50.f, 255.f});
    }

    DrawEntity((Rectangle){state.player.entity.col.x, state.player.entity.col.y, state.player.entity.col.width, state.player.entity.col.height, state.player.entity.tex.ID});

    // 3D
    DrawCube((vec3){1, 1.5, 0.5f}, (vec3){1, 1, 1}, (Vector3){p * 20, 0, p * 20});
    DrawCube((vec3){5, 2, 0.5f}, (vec3){5, 1, 1}, (Vector3){40, 40.f, 20.f});
    // DrawCube((vec3){Boxes[1].x, Boxes[1].y, 0.5f}, (vec3){Boxes[1].width, Boxes[1].height, 1}, (Vector3){0.f, 0.f, 0.f});
    // DrawCube((vec3){Boxes[2].x, Boxes[2].y, 0.5f}, (vec3){Boxes[2].width, Boxes[2].height, 1}, (Vector3){0.f, 0.f, 0.f});

    // UI
    DrawRectangleBasic((Rectangle){state.mouse_world.x, state.mouse_world.y, 0.25f, 0.25f, 0}, (vec4){255.f, 50.f, 50.f, 255.f});
    DrawLine((Vector2){state.player.entity.col.x, state.player.entity.col.y}, state.mouse_world, (vec4){50.f, 50.f, 50.f, 255.f});
    DrawLine((Vector2){state.player.entity.col.x, state.player.entity.col.y}, (Vector2){state.player.entity.col.x + state.player.entity.velocity.x * frame_time * 20, state.player.entity.col.y + state.player.entity.velocity.y * frame_time * 20}, (vec4){255.f, 50.f, 50.f, 255.f});
    DrawRectangleBasic((Rectangle){intersectionX, intersectionY, 0.25f, 0.25f, 0}, (vec4){0.f, 0.f, 0.f, 255.f});

    glBindFramebuffer(GL_FRAMEBUFFER, post_process_FBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    UseShader(advanced);
    SetShaderInt(advanced.ID, "g_position", 0);
    SetShaderInt(advanced.ID, "g_normal", 1);
    SetShaderInt(advanced.ID, "g_albedo", 2); // bloom.mip_chain[0].texture.ID);
    SetShaderVec2(advanced.ID, "resolution", (vec2){screen_width, screen_height});
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_position);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, g_normal);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, g_albedo);
    SetShaderVec3(advanced.ID, "view_pos", (vec3){state.camera.position.x, state.camera.position.y, state.camera.position.z});
    DrawQuad();
    RenderBloom(post_process_color, bloom_filter_radius);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    UseShader(post_process_shader);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, post_process_color);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, bloom.mip_chain[0].texture.ID);
    if (false)
    {
        glGenerateMipmap(GL_TEXTURE_2D); // Generate mipmaps every frame
        vec3 luminescence;
        glGetTexImage(GL_TEXTURE_2D, 10, GL_RGB, GL_FLOAT, &luminescence);                                   // Read the value from the lowest mip level
        const float lum = 0.2126f * luminescence[0] + 0.7152f * luminescence[1] + 0.0722f * luminescence[2]; // Calculate a weighted average

        const float adjSpeed = 2.5f;
        scene_exposure = glm_lerp(scene_exposure, 0.5f / lum * 1.f, adjSpeed * frame_time); // Gradually adjust the exposure
        scene_exposure = glm_clamp(scene_exposure, 0.01f, 4.f);                             // Don't let it go over or under a specified min/max range
    }
    SetShaderInt(post_process_shader.ID, "lighting", 0);
    SetShaderInt(post_process_shader.ID, "bloom", 1);

    SetShaderFloat(post_process_shader.ID, "exposure", 1.0); //1.0f - (scene_exposure-4));
    SetShaderFloat(post_process_shader.ID, "bloom_strength",  0.05f); //scene_exposure/2 * 0.05f);
    DrawQuad();
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