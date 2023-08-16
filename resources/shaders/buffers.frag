#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform vec4 color;
uniform bool use_color;
uniform sampler2D tex;

void main()
{    
    // store the fragment position vector in the first gbuffer texture
    gPosition = FragPos; //vec3(1, 0, 0);
    // also store the per-fragment normals into the gbuffer
    gNormal = normalize(Normal);
    // and the diffuse per-fragment color
    if (!use_color)
        gAlbedoSpec.rgb = texture(tex, TexCoords).rgb;
    else
    {
        gAlbedoSpec.rgb = color.rgb;
    }
    // store specular intensity in gAlbedoSpec's alpha component
    gAlbedoSpec.a = 1;
}  