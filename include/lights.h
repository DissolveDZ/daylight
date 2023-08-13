#include <math.h>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>
#include <cglm/call.h>
#include <string.h>
#include <shader.h> // Assuming this header has GLM's vec3 definition and other necessary headers
#pragma once

// Maximum number of point lights
#define MAX_POINT_LIGHTS 10

typedef struct PointLight
{
    vec3 position;
    float padding1;
    vec3 color;
    float padding2;
    vec3 ambient;
    float padding3;
} PointLight;

typedef struct LightUBO
{
    PointLight point_lights;
    int point_light_count;
} LightUBO;

GLuint lights_ubo;
GLuint lights_index = 0;
GLuint lights_binding_point = 0;
LightUBO light_ubo_data;
static inline void InitLights()
{
    // Initialize light UBO data
    // light_ubo_data = malloc(sizeof(LightUBO * 10));
    light_ubo_data.point_light_count = 0;
    // light_ubo_data.point_lights = malloc(sizeof(PointLight));
    //lights_index = glGetUniformBlockIndex(0, "LightUBO");
    glUniformBlockBinding(0, lights_index, lights_binding_point);
    glGenBuffers(1, &lights_ubo);
    glBindBufferBase(GL_UNIFORM_BUFFER, lights_binding_point, lights_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, lights_ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(PointLight) * MAX_POINT_LIGHTS + sizeof(int), &light_ubo_data, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, lights_binding_point, lights_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    // glBindBufferRange(GL_UNIFORM_BUFFER, lights_binding_point, lights_ubo, 0, sizeof(PointLight) * max + sizeof(int));
}

PointLight *CreatePointLight(vec3 position, vec3 color, vec3 ambient)
{
    glBindBuffer(GL_UNIFORM_BUFFER, lights_ubo);
    glGetBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(LightUBO), &light_ubo_data);

    if (light_ubo_data.point_light_count >= MAX_POINT_LIGHTS)
    {
        fprintf(stderr, "Max point lights limit reached!\n");
        return NULL;
    }

    PointLight *light = &light_ubo_data.point_lights; //[light_ubo_data.point_light_count];

    // Assign values directly to the struct members
    glm_vec3_copy(position, light->position);
    glm_vec3_copy(color, light->color);
    glm_vec3_copy(ambient, light->ambient);

    light_ubo_data.point_light_count++;
    return light;
}

static inline void UpdateLights()
{
    // Update the point light data in the UBO buffer
    glBindBuffer(GL_UNIFORM_BUFFER, lights_ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(LightUBO), &light_ubo_data);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

static inline void FreeLights()
{
    // free(light_ubo_data.point_lights);
    // point_lights_data = NULL;
    glDeleteBuffers(1, &lights_ubo);
}
