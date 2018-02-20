#version 330 core

in vec3 vertexPos;
in vec2 UV;

out vec4 FragColor;

uniform sampler2D color;
uniform sampler2D normals;

void main()
{
    FragColor = texture(color, UV);
}