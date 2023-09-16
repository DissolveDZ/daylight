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

void DrawRectangleBasic(Rectangle rec, vec4 color)
{
    // Skip matrix when not moving
    UseShader(basic);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);

    SetShaderMat4(basic.ID, "projection", projection);
    SetShaderMat4(basic.ID, "view", view);
    SetShaderBool(basic.ID, "use_color", true);
    SetShaderBool(basic.ID, "use_normals", false);
    SetShaderVec4(basic.ID, "color", (vec4){color[0] / 255, color[1] / 255, color[2] / 255, color[3] / 255});
    SetShaderFloat(basic.ID, "exposure", 1);
    SetShaderBool(basic.ID, "hdr", true);

    glm_mat4_identity(model);
    glm_translate(model, (vec3){rec.x, rec.y, 0.0f});
    glm_scale(model, (vec3){rec.width, rec.height, 1});
    SetShaderMat4(basic.ID, "model", model);

    glBindVertexArray(planeVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void DrawEntity(Rectangle rec)
{
    // Skip matrix when not moving
    UseShader(basic);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, rec.texture_id);
    SetShaderMat4(basic.ID, "projection", projection);
    SetShaderMat4(basic.ID, "view", view);
    SetShaderBool(basic.ID, "use_color", false);
    SetShaderBool(basic.ID, "use_normals", false);
    SetShaderVec3(basic.ID, "view_pos", (vec3){state.camera.position.x, state.camera.position.y, state.camera.position.z});
    SetShaderBool(basic.ID, "flip", state.player.entity.flip);
    SetShaderFloat(basic.ID, "exposure", 1);
    SetShaderBool(basic.ID, "hdr", true);

    glm_mat4_identity(model);
    glm_translate(model, (vec3){rec.x, rec.y, 0.0f});
    glm_scale(model, (vec3){rec.width, rec.height, 1});
    SetShaderMat4(basic.ID, "model", model);

    glBindVertexArray(planeVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void DrawCube(vec3 position, vec3 scale, Vector3 rotation)
{
    glEnable(GL_DEPTH_TEST);
    UseShader(geometry_shader);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, cube.ID);
    SetShaderMat4(geometry_shader.ID, "projection", projection);
    SetShaderMat4(geometry_shader.ID, "view", view);
    SetShaderBool(geometry_shader.ID, "use_color", false);
    SetShaderBool(geometry_shader.ID, "use_normals", true);
    SetShaderVec3(geometry_shader.ID, "view_pos", (vec3){state.camera.position.x, state.camera.position.y, state.camera.position.z});
    SetShaderFloat(geometry_shader.ID, "exposure", 1);
    SetShaderBool(geometry_shader.ID, "hdr", true);

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
    SetShaderMat4(geometry_shader.ID, "model", model);

    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDisable(GL_DEPTH_TEST);
}

void DrawRectangle(Rectangle rec)
{
    // Skip matrix when not moving
    UseShader(basic);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, rec.texture_id);

    SetShaderMat4(basic.ID, "projection", projection);
    SetShaderMat4(basic.ID, "view", view);
    SetShaderBool(basic.ID, "use_color", false);
    SetShaderBool(basic.ID, "use_normals", false);
    SetShaderVec3(basic.ID, "view_pos", (vec3){state.camera.position.x, state.camera.position.y, state.camera.position.z});
    SetShaderFloat(basic.ID, "exposure", 1);
    SetShaderBool(basic.ID, "hdr", true);

    glm_mat4_identity(model);
    glm_translate(model, (vec3){rec.x, rec.y, 0.0f});
    glm_scale(model, (vec3){rec.width, rec.height, 1});
    SetShaderMat4(basic.ID, "model", model);

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

void LightingPass()
{
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
}