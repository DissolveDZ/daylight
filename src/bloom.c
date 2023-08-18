#include "main.h"
#define MAX_BLOOM_MIP 10

void BloomInit(int mip_amount)
{
    glGenFramebuffers(1, &bloom.FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, bloom.FBO);
    if (mip_amount > MAX_BLOOM_MIP)
        mip_amount = MAX_BLOOM_MIP;
    vec2 mip_size = {(float)screen_width, (float)screen_height};
    ivec2 mip_int_size = {(int)screen_width, (int)screen_height};
    if (screen_width > (unsigned int)INT_MAX || screen_height > (unsigned int)INT_MAX) {
		printf("Window size conversion overflow - cannot build bloom FBO!");
		return false;
	}
    bloom.mip_chain_len += mip_amount;
    bloom.mip_chain = malloc(sizeof(BloomMip) * bloom.mip_chain_len);
    for (unsigned int i = 0; i < mip_amount; i++)
    {
        BloomMip mip;
        glm_vec2_mul(mip_size, (vec2){0.5f, 0.5f}, mip_size);
        glm_vec2_div(mip_int_size, (ivec2){2, 2}, mip_int_size);
        glm_vec2_copy(mip_size, mip.size);
        glm_ivec2_copy(mip_int_size, mip.int_size);

        glGenTextures(1, &mip.texture.ID);
        glBindTexture(GL_TEXTURE_2D, mip.texture.ID);
        // we are downscaling an HDR color buffer, so we need a float texture format
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R11F_G11F_B10F,
                     (int)mip_size[0], (int)mip_size[1],
                     0, GL_RGB, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        bloom.mip_chain[i] = mip;
    }
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, bloom.mip_chain[0].texture.ID, 0);

    // setup attachments
    unsigned int attachments[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, attachments);

    // check completion status
    int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        printf("gbuffer FBO error, status: 0x\%x\n", status);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    upsample_shader = LoadShader("resources/shaders/vertex.vert", "resources/shaders/upsample.frag");
    downsample_shader = LoadShader("resources/shaders/vertex.vert", "resources/shaders/downsample.frag");

    UseShader(upsample_shader);
    SetShaderInt(upsample_shader.ID, "src_texture", 0);

    UseShader(downsample_shader);
    SetShaderInt(downsample_shader.ID, "src_texture", 0);
}

void UpsampleBloom(float filter_radius)
{
    UseShader(upsample_shader);
    SetShaderFloat(upsample_shader.ID, "filter_radius", filter_radius);

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    glBlendEquation(GL_FUNC_ADD);
    for (int i = (int)bloom.mip_chain_len - 1; i > 0; i--)
    {
        const BloomMip *mip = &bloom.mip_chain[i];
        const BloomMip *next_mip = &bloom.mip_chain[i-1];
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mip->texture.ID);

        glViewport(0, 0, next_mip->size[0], next_mip->size[1]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, next_mip->texture.ID, 0);

        // Render screen-filled quad of resolution of current mip
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);
    }
    glDisable(GL_BLEND);
    glUseProgram(0);
}

void DownSampleBloom(unsigned int src_texture)
{
    UseShader(downsample_shader);
    SetShaderVec2(downsample_shader.ID, "src_resolution", (vec2){(float)screen_width, (float)screen_height});

    // Bind srcTexture (HDR color buffer) as initial texture input
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, src_texture);

    // Progressively downsample through the mip chain
    for (int i = 0; i < bloom.mip_chain_len; i++)
    {
        BloomMip *mip = &bloom.mip_chain[i];
        glViewport(0, 0, mip->size[0], mip->size[1]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_2D, mip->texture.ID, 0);

        // Render screen-filled quad of resolution of current mip
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);

        // Set current mip resolution as srcResolution for next iteration
        SetShaderVec2(downsample_shader.ID, "src_resolution", mip->size);
        // Set current mip as texture input for next iteration
        glBindTexture(GL_TEXTURE_2D, mip->texture.ID);
    }
}

void RenderBloom(unsigned int src_texture, float filter_radius)
{
    glBindFramebuffer(GL_FRAMEBUFFER, bloom.FBO);
    DownSampleBloom(src_texture);
    UpsampleBloom(filter_radius);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, screen_width, screen_height);
}