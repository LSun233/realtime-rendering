#version 330 core
layout (location = 0) out vec3 glPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gAlbedo;

uniform vec3 albedo;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

void main()
{   
    // store the fragment position vector in the first gbuffer texture
      glPosition = FragPos;
    // also store the per-fragment normals into the gbuffer
      gNormal = normalize(Normal);
    // and the diffuse per-fragment color
    gAlbedo.rgb = albedo;


}