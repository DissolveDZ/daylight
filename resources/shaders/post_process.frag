#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D lighting;
uniform sampler2D bloom;

uniform float exposure;
uniform float bloom_strength = 0.04f;

vec3 hdr_color = texture(lighting, TexCoords).rgb;
vec3 bloom_color = texture(bloom, TexCoords).rgb;
vec3 color = mix(hdr_color, bloom_color, bloom_strength);

void main()
{
    color = vec3(1.0) - exp(-color * exposure);
    color = pow(color, vec3(1.0 / 2.2));
    FragColor = vec4(color, 1.0);
}