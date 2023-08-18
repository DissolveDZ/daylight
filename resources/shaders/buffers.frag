#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform vec3 view_pos;
uniform vec4 color;
uniform bool use_color;
uniform bool flip;
uniform sampler2D tex;

void main()
{    
    // and the diffuse per-fragment color
    gAlbedoSpec = vec4(1);
    if (!use_color)
    {
        if (flip)
            gAlbedoSpec = texture(tex, vec2(1.0 - TexCoords.x, TexCoords.y));
        else
            gAlbedoSpec = texture(tex, TexCoords);
        if (gAlbedoSpec.a == 0.0)
            discard;
        if (!use_color)
        {
            gAlbedoSpec.rgb = pow(gAlbedoSpec.rgb, vec3(2.2));
        }
    }
    else
    {
        gAlbedoSpec.rgb = color.rgb;
    }
    // store the fragment position vector in the first gbuffer texture
    gPosition = FragPos;
    // also store the per-fragment normals into the gbuffer
    gNormal = normalize(Normal);
    // store specular intensity in gAlbedoSpec's alpha component
}  