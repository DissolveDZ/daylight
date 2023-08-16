#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec3 FragPos;
out vec2 TexCoords;
out vec3 Normal;

uniform bool use_normals;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
    gl_Position = vec4(aPos, 1.0);
    TexCoords = aTexCoords;
}