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
    // ACES
    /*
    float a = 2.51f;
    float b = 0.03f;
    float c = 2.43f;
    float d = 0.59f;
    float e = 0.14f;
    color = clamp((color*(a*color+b))/(color*(c*color+d)+e), 0.0, 1.0);
    */

    //color = vec3(1.0) - exp(-color * exposure);

    // Reinhard
    color = color * (1.0 + color / (exposure * exposure)) / (1.0 + color);

    // Gamma Correction
    color = pow(color, vec3(1.0 / 2.2));

    FragColor = vec4(color, 1.0);
}