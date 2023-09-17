// #include "collisions.c"
float p = 0;
float bloom_filter_radius = 0.004f;

void DrawLine(Vector2 line_start, Vector2 line_end, vec4 color)
{
    UseShader(color_shader);

    SetShaderMat4(color_shader.ID, "projection", projection);
    SetShaderMat4(color_shader.ID, "view", view);
    SetShaderVec4(color_shader.ID, "color", (vec4){color[0] / 255, color[1] / 255, color[2] / 255, color[3] / 255});
    SetShaderFloat(color_shader.ID, "exposure", 1);
    SetShaderBool(color_shader.ID, "hdr", true);
    glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(line_vertices), line_vertices, GL_STATIC_DRAW);
    glm_mat4_identity(model);
    line_vertices[0] = line_start.x;
    line_vertices[1] = line_start.y;
    line_vertices[2] = 0;
    line_vertices[3] = line_end.x;
    line_vertices[4] = line_end.y;
    line_vertices[5] = 0;
    SetShaderMat4(color_shader.ID, "model", model);
    glBindVertexArray(lineVAO);
    glDrawArrays(GL_LINES, 0, 2);
    glBindVertexArray(0);
}

void DrawRectangleBasic(Rectangle rec, vec4 color, Shader shader)
{
    // Skip matrix when not moving
    UseShader(shader);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);

    SetShaderMat4(shader.ID, "projection", projection);
    SetShaderMat4(shader.ID, "view", view);
    SetShaderBool(shader.ID, "use_color", true);
    SetShaderBool(shader.ID, "use_normals", false);
    SetShaderVec4(shader.ID, "color", (vec4){color[0] / 255, color[1] / 255, color[2] / 255, color[3] / 255});
    SetShaderFloat(shader.ID, "exposure", 1);
    SetShaderBool(shader.ID, "hdr", true);

    glm_mat4_identity(model);
    glm_translate(model, (vec3){rec.x, rec.y, 0.0f});
    glm_scale(model, (vec3){rec.width, rec.height, 1});
    SetShaderMat4(shader.ID, "model", model);

    glBindVertexArray(planeVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void DrawEntity(Rectangle rec, Shader shader)
{
    // Skip matrix when not moving
    UseShader(shader);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, rec.texture_id);
    SetShaderMat4(shader.ID, "projection", projection);
    SetShaderMat4(shader.ID, "view", view);
    SetShaderBool(shader.ID, "use_color", false);
    SetShaderBool(shader.ID, "use_normals", false);
    SetShaderVec3(shader.ID, "view_pos", (vec3){state.camera.position.x, state.camera.position.y, state.camera.position.z});
    SetShaderBool(shader.ID, "flip", state.player.entity.flip);
    SetShaderFloat(shader.ID, "exposure", 1);
    SetShaderBool(shader.ID, "hdr", true);

    glm_mat4_identity(model);
    glm_translate(model, (vec3){rec.x, rec.y, 0.0f});
    glm_scale(model, (vec3){rec.width, rec.height, 1});
    SetShaderMat4(shader.ID, "model", model);

    glBindVertexArray(planeVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void DrawCube(vec3 position, vec3 scale, Vector3 rotation, Texture texture, Shader shader)
{
    glEnable(GL_DEPTH_TEST);
    UseShader(shader);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture.ID);
    SetShaderMat4(shader.ID, "projection", projection);
    SetShaderMat4(shader.ID, "view", view);
    SetShaderBool(shader.ID, "use_color", false);
    SetShaderBool(shader.ID, "use_normals", true);
    SetShaderVec3(shader.ID, "view_pos", (vec3){state.camera.position.x, state.camera.position.y, state.camera.position.z});
    SetShaderFloat(shader.ID, "exposure", 1);
    SetShaderBool(shader.ID, "hdr", true);

    glm_mat4_identity(model);
    mat4 temp;
    glm_mat4_identity(temp);
    versor quater;
    glm_quat_identity(quater);
    mat4 idendidy;
    glm_mat4_identity(idendidy);
    mat4 transformx;
    glm_mat4_identity(transformx);
    glm_rotate(transformx, glm_rad(rotation.x), (vec3){1.f, 0.f, 0.f});
    mat4 transformy;
    glm_mat4_identity(transformy);
    glm_rotate(transformy, glm_rad(rotation.y), (vec3){0.f, 1.f, 0.f});
    mat4 transformz;
    glm_mat4_identity(transformz);
    glm_rotate(transformz, glm_rad(rotation.z), (vec3){0.f, 0.f, 1.f});
    mat4 rotationmat;
    glm_mat4_identity(rotationmat);
    glm_mul(transformy, transformx, temp);
    glm_mul(temp, transformz, rotationmat);

    // glm_quat_from_vecs((vec3){0, 0, 0}, (vec3){glm_rad(rotation.y), glm_rad(rotation.x), glm_rad(rotation.z)}, quater);
    // glm_quat_mat4(quater, temp);
    glm_translate(model, position);
    glm_mat4_mul(model, rotationmat, model);
    glm_scale(model, scale);
    // glm_rotate(model, glm_rad((float)SDL_GetTicks64() / 50), rotation);
    SetShaderMat4(shader.ID, "model", model);

    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDisable(GL_DEPTH_TEST);
}

void DrawRectangle(Rectangle rec, Shader shader)
{
    // Skip matrix when not moving
    UseShader(shader);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, rec.texture_id);

    SetShaderMat4(shader.ID, "projection", projection);
    SetShaderMat4(shader.ID, "view", view);
    SetShaderBool(shader.ID, "use_color", false);
    SetShaderBool(shader.ID, "use_normals", false);
    SetShaderVec3(shader.ID, "view_pos", (vec3){state.camera.position.x, state.camera.position.y, state.camera.position.z});
    SetShaderFloat(shader.ID, "exposure", 1);
    SetShaderBool(shader.ID, "hdr", true);

    glm_mat4_identity(model);
    glm_translate(model, (vec3){rec.x, rec.y, 0.0f});
    glm_scale(model, (vec3){rec.width, rec.height, 1});
    SetShaderMat4(shader.ID, "model", model);

    glBindVertexArray(planeVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

// Draw a circle to a quad
void DrawCircle(vec3 position, float radius, vec4 color)
{
    UseShader(circle_shader);

    SetShaderMat4(circle_shader.ID, "projection", projection);
    SetShaderMat4(circle_shader.ID, "view", view);
    glm_mat4_identity(model);
    glm_translate(model, position);
    glm_scale(model, (vec3){radius, radius, 1});
    SetShaderMat4(circle_shader.ID, "model", model);
    SetShaderVec3(circle_shader.ID, "view_pos", (vec3){state.camera.position.x, state.camera.position.y, state.camera.position.z});
    SetShaderFloat(circle_shader.ID, "edge_blur", 0.05f);
    SetShaderVec4(circle_shader.ID, "color", (vec4){color[0] / 255, color[1] / 255, color[2] / 255, color[3] / 255});
    SetShaderVec2(circle_shader.ID, "center", (vec2){position[0], position[1]});
    SetShaderFloat(circle_shader.ID, "radius", radius);

    glBindVertexArray(planeVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}


void Draw()
{
    p += frame_time;
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);
    glBindFramebuffer(GL_FRAMEBUFFER, g_buffer);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glClearStencil(0x00);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    vec3 temp;
    glm_mat4_identity(view);
    vec3 camera_pos = {state.camera.position.x, state.camera.position.y, state.camera.position.z};
    glm_vec3_add(camera_pos, (vec3){0, 0, -1}, temp);
    glm_lookat(camera_pos, temp, (vec3){0, 1, 0}, view);

    // Sun
    DrawCircle((vec3){state.camera.position.x - 1.5f, state.camera.position.y + 1.f, state.camera.position.z - 5}, 0.5f, (vec4){255.f * 10, 255.f * 10, 255.f * 10, 255.f});

    for (int i = 0; i < MAX_BUILDINGS; i++)
    {
        DrawRectangleBasic(buildings[i], buildColors[i], basic);
    }
    for (int i = 0; i < state.cur_colliders; i++)
    {
        DrawRectangleBasic((Rectangle){Boxes[i].x, Boxes[i].y, Boxes[i].width, Boxes[i].height, 0}, (vec4){50.f, 180.f, 50.f, 255.f}, basic);
    }

    DrawEntity((Rectangle){state.player.entity.col.x, state.player.entity.col.y, state.player.entity.col.width, state.player.entity.col.height, state.player.entity.tex.ID}, basic);

    // 3D

    DrawCube((vec3){1, 1.5, 0.5f}, (vec3){1, 1, 1}, (Vector3){p * 20, 0, p * 20}, cube, geometry_shader);
    DrawCube((vec3){5, 2, 0.5f}, (vec3){5, 1, 1}, (Vector3){40, 40.f, 20.f}, cube, geometry_shader);

    // UI

    DrawRectangleBasic((Rectangle){state.mouse_world.x, state.mouse_world.y, 0.25f, 0.25f, 0}, (vec4){255.f, 50.f, 50.f, 255.f}, basic);
    DrawLine((Vector2){state.player.entity.col.x, state.player.entity.col.y}, state.mouse_world, (vec4){50.f, 50.f, 50.f, 255.f});
    DrawLine((Vector2){state.player.entity.col.x, state.player.entity.col.y}, (Vector2){state.player.entity.col.x + state.player.entity.velocity.x * frame_time * 20, state.player.entity.col.y + state.player.entity.velocity.y * frame_time * 20}, (vec4){255.f, 50.f, 50.f, 255.f});
    DrawRectangleBasic((Rectangle){intersectionX, intersectionY, 0.25f, 0.25f, 0}, (vec4){0.f, 0.f, 0.f, 255.f}, basic);
    glStencilMask(0x00);

    // LIGHTING PASS

    glBindFramebuffer(GL_FRAMEBUFFER, post_process_fbo);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    UseShader(sky_shader);
    SetShaderVec2(sky_shader.ID, "resolution", (vec2){screen_width, screen_height});
    SetShaderVec3(sky_shader.ID, "view_pos", (vec3){state.camera.position.x, state.camera.position.y, state.camera.position.z});
    DrawQuad();

    glStencilFunc(GL_EQUAL, 1, 0xFF);
    UseShader(advanced);
    SetShaderInt(advanced.ID, "g_position", 0);
    SetShaderInt(advanced.ID, "g_normal", 1);
    SetShaderInt(advanced.ID, "g_albedo", 2);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_position);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, g_normal);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, g_albedo);
    SetShaderVec3(advanced.ID, "view_pos", (vec3){state.camera.position.x, state.camera.position.y, state.camera.position.z});
    DrawQuad();
    glDisable(GL_STENCIL_TEST);

    // POST PROCESS

    // Bloom
    RenderBloom(post_process_color, bloom_filter_radius, 1.0f, 1.0f, &bloom, quadVAO);

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

    SetShaderFloat(post_process_shader.ID, "exposure", 1.0);         // 1.0f - (scene_exposure-4));
    SetShaderFloat(post_process_shader.ID, "bloom_strength", 0.05f); // scene_exposure/2 * 0.05f);
    DrawQuad();
    // printf("advanced shader: %i\n", basic.ID);

    // send light relevant uniforms
    SDL_GL_SwapWindow(main_window);
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