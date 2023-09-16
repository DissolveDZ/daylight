void paintest()
{
    //printf("PAY");
    //printf("%i", screen_width);
}

int GetRandomValue(int min, int max)
{
    if (min > max)
    {
        int tmp = max;
        max = min;
        min = tmp;
    }
    if ((unsigned int)(max - min) > (unsigned int)RAND_MAX)
    {
        printf("value exceeds %i", RAND_MAX);
    }
    return (rand() % (abs(max - min) + 1) + min);
}

float Lerp(float start, float end, float amount)
{
    float result = start + amount * (end - start);

    return result;
}

Rectangle *CreateRectangle(int x, int y, int width, int height, Texture tex)
{
    Rectangle *rec = malloc(sizeof(Rectangle));
    *rec = (Rectangle){x, y, width, height, tex.ID};
    if (state.recs_len + 1 > state.recs_max)
    {
        state.recs_max += 20;
        state.recs = realloc(state.recs, state.recs_max);
    }
    state.recs[state.recs_len] = rec;
    state.recs_len++;
    return rec;
}

Collider RecToCollider(Rectangle rec, bool rotating, bool dynamic)
{
    Collider col;
    if (rotating)
    {
        col.vertices = malloc(sizeof(Line) * 2);
        col.vertices[0] = (Line){(Vector2){0, 0}, (Vector2){0 + rec.width, rec.y}};
        col.vertices[1] = (Line){(Vector2){0, rec.height}, (Vector2){rec.width, rec.height}};
    }
    col.x = rec.x;
    col.y = rec.y;
    col.width = rec.width;
    col.height = rec.height;
    col.rotating = rotating;
    col.dynamic = dynamic;
    state.cur_colliders++;
    return col;
}

void DrawQuad()
{
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

Vector3 Vector3Transform(vec3 v, mat4 mat)
{
    Vector3 result;
    float x = v[0];
    float y = v[1];
    float z = v[2];
    result.x = mat[0][0] * x + mat[1][0] * y + mat[2][0] * z + mat[3][0];
    result.y = mat[0][1] * x + mat[1][1] * y + mat[2][1] * z + mat[3][1];
    result.z = mat[0][2] * x + mat[1][2] * y + mat[2][2] * z + mat[3][2];
    return result;
}

Vector2 GetScreenToWorld2D(Vector2 position)
{
    Vector2 clipcoord = {2.0f * position.x / (float)screen_width - 1.0f, 1.0 - 2.0 * position.y / (float)screen_height};
    mat4 inv_view;
    glm_mat4_inv(projection, inv_view);
    Vector3 transform;
    // glm_scale(invmat, transform);
    transform = Vector3Transform((vec3){clipcoord.x, clipcoord.y, 1}, inv_view);
    // transform = (vec4){position.x, position.y, 1, 1} * inv_view;
    return (Vector2){transform.x * state.camera.position.z + state.camera.position.x, transform.y * state.camera.position.z + state.camera.position.y};
}

void OnResize(int new_width, int new_height)
{
    if (new_width > (int)INT_MAX || new_height > (int)INT_MAX)
    {
        printf("Window size overflow");
        return false;
    }
    screen_width = new_width;
    screen_height = new_height;
    glBindTexture(GL_TEXTURE_2D, g_position);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screen_width, screen_height, 0, GL_RGBA, GL_FLOAT, NULL);
    glBindTexture(GL_TEXTURE_2D, g_normal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screen_width, screen_height, 0, GL_RGBA, GL_FLOAT, NULL);
    glBindTexture(GL_TEXTURE_2D, g_albedo);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screen_width, screen_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, post_process_color);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screen_width, screen_height, 0, GL_RGBA, GL_FLOAT, NULL);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screen_width, screen_height);
    glViewport(0, 0, screen_width, screen_height);
    if (bloom.enabled)
    {
        vec2 mip_size = {(float)screen_width, (float)screen_height};
        ivec2 mip_int_size = {(int)screen_width, (int)screen_height};
        for (unsigned int i = 0; i < bloom.mip_chain_len; i++)
        {
            BloomMip *mip = &bloom.mip_chain[i];
            glm_vec2_mul(mip_size, (vec2){0.5f, 0.5f}, mip_size);
            glm_vec2_div(mip_int_size, (ivec2){2, 2}, mip_int_size);
            glm_vec2_copy(mip_size, mip->size);
            glm_ivec2_copy(mip_int_size, mip->int_size);
            glBindTexture(GL_TEXTURE_2D, mip->texture.ID);
            // we are downscaling an HDR color buffer, so we need a float texture format
            glTexImage2D(GL_TEXTURE_2D, 0, GL_R11F_G11F_B10F,
                         (int)mip_size[0], (int)mip_size[1],
                         0, GL_RGB, GL_FLOAT, NULL);
        }
    }
}