#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 textureCoord;

out vec3 vertPos;
out vec3 vertColor;
out vec2 vertTextureCoord;

void main()
{
    gl_Position = vec4(pos, 1.0);
    vertPos = pos;
    vertColor = color;
    vertTextureCoord = textureCoord;
}
