#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

vec4 color;

void main()
{   
    FragColor = texture(texture_diffuse1, TexCoords);
    color = texture(texture_specular1, TexCoords);
}