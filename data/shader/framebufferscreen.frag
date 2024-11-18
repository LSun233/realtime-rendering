#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;

//

void main()
{
    vec3 col0 = texture(gPosition, TexCoords).xyz;
    vec3 col1 = texture(gNormal, TexCoords).xyz;
    vec3 col2 = texture(gAlbedo, TexCoords).xyz;

    FragColor = vec4(col2, 1.0);
} 