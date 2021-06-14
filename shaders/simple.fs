#version 330 core

in vec3 vertPos;
in vec3 vertColor;
in vec2 vertTextureCoord;

out vec4 FragColor;

uniform sampler2D textureSampler0;
uniform sampler2D textureSampler1;

void main()
{
    FragColor = mix(texture(textureSampler0, vertTextureCoord), texture(textureSampler1, vertTextureCoord), 0.7);
}