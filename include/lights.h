#include <math.h>
#include <stddef.h> // Include this for offsetof
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

int point_light_count;

PointLight *point_lights;
GLuint lights_ubo;
GLuint lights_index = 0;
GLuint lights_binding_point = 0;
static inline void InitLights()
{
    point_light_count = 0;
    point_lights = malloc(sizeof(PointLight) * 10);
    glGenBuffers(1, &lights_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, lights_ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(PointLight)*MAX_POINT_LIGHTS + sizeof(vec4), NULL, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, lights_ubo);
    //glBindBufferRange(GL_UNIFORM_BUFFER, 0, lights_ubo, 0, 496);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

PointLight *CreatePointLight(vec3 position, vec3 color, vec3 ambient)
{
    if (point_light_count >= MAX_POINT_LIGHTS)
    {
        fprintf(stderr, "Max point lights limit reached!\n");
        return NULL;
    }

    PointLight *light = &point_lights[point_light_count];

    // Assign values directly to the struct members
    glm_vec3_copy(position, light->position);
    glm_vec3_copy(color, light->color);
    glm_vec3_copy(ambient, light->ambient);

    point_light_count++;
    return light;
}
int tesst = 1;
static inline void UpdateLights()
{
    glBindBuffer(GL_UNIFORM_BUFFER, lights_ubo);
    // Update the point light count in the UBO buffer
    //glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(long), &point_light_count);
    
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(PointLight) * point_light_count, point_lights);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(PointLight) * MAX_POINT_LIGHTS, sizeof(long), &point_light_count);
    
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

static inline void FreeLights()
{
    // free(light_ubo_data.point_lights);
    // point_lights_data = NULL;
    // glDeleteBuffers(1, &lights_ubo);
}
