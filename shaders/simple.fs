#version 330 core

in vec3 outPos;

out vec4 FragColor;

void main()
{
    FragColor = vec4(outPos.xyz / 2.0 + 0.5, 1.0);
}