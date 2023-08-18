#include <math.h>
#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>
#include "general.h"
#pragma once

// Function to format the shader uniform string
// and return a dynamically allocated string.
static char* FormatShaderUniform(const char* uniform_name, int index)
{
    int buffer_size = snprintf(NULL, 0, "%s[%d].%s", uniform_name, index, uniform_name) + 1;
    char* buffer = malloc(buffer_size);
    snprintf(buffer, buffer_size, "%s[%d].%s", uniform_name, index, uniform_name);
    return buffer;
}

typedef struct Shader
{
    unsigned int ID;
} Shader;

static inline Shader LoadShader(char *vertex_path, char *fragment_path)
{
    Shader shader = {0};
    char* vertex_shader_source = ReadTextFile(vertex_path);
    char* fragment_shader_source = ReadTextFile(fragment_path);

    // compile shaders
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    // vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n %s", infoLog);
    };
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n %s", infoLog);
    };
    shader.ID = glCreateProgram();
    glAttachShader(shader.ID, vertex);
    glAttachShader(shader.ID, fragment);
    glLinkProgram(shader.ID);
    glGetProgramiv(shader.ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shader.ID, 512, NULL, infoLog);
        printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n %s", infoLog);
    }
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return shader;
}
static inline void UseShader(Shader shader)
{
    glUseProgram(shader.ID);
}
static inline void SetShaderBool(int Shader_ID, const char *name, bool value)
{
    glUniform1i(glGetUniformLocation(Shader_ID, name), (int)value);
}
static inline void SetShaderInt(int Shader_ID, const char *name, int value)
{
    glUniform1i(glGetUniformLocation(Shader_ID, name), value);
}
static inline void SetShaderFloat(int Shader_ID, const char *name, float value)
{
    glUniform1f(glGetUniformLocation(Shader_ID, name), value);
}
static inline void SetShaderMat4(int Shader_ID, const char *name, mat4 matrix)
{
    glUniformMatrix4fv(glGetUniformLocation(Shader_ID, name), 1, GL_FALSE, matrix);
}
static inline void SetShaderVec2(int Shader_ID, const char *name, vec2 vector)
{
    glUniform2f(glGetUniformLocation(Shader_ID, name), vector[0], vector[1]);
}
static inline void SetShaderVec3(int Shader_ID, const char *name, vec3 vector)
{
    glUniform3f(glGetUniformLocation(Shader_ID, name), vector[0], vector[1], vector[2]);
}

static inline void SetShaderVec4(int Shader_ID, const char *name, vec4 value)
{
    glUniform4f(glGetUniformLocation(Shader_ID, name), value[0], value[1], value[2], value[3]);
}

static inline void SetShaderVec3v(int Shader_ID, const char *name, vec3 *vector, int amount)
{
    glUniform3fv(glGetUniformLocation(Shader_ID, name), amount, vector);
}