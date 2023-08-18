#version 460 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform vec2 center;
uniform vec3 view_pos;
uniform vec4 color;
uniform float edge_blur;
uniform float radius;
void main()
{

    float distance = 1.0 - length(center - view_pos.xy - FragPos.xy) + (radius / 2.0 - 1.0);
    float circle = distance *5; //smoothstep(0.0, 0.05, distance);
    if (circle <= 0.001)
        discard;
    gAlbedoSpec = vec4(circle * color.rgb, 1);
    gNormal = normalize(Normal);
    gPosition = FragPos;
}