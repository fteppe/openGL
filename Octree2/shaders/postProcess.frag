#version 330 core

in vec3 vertexPos;
in vec2 UV;

out vec4 FragColor;

void main()
{
    FragColor = vec4(vertexPos,1);
}