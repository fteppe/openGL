#version 330 core
in vec4 vertexColor;
out vec4 FragColor;

uniform float[7] light;

void main()
{
    FragColor = vertexColor;
} 