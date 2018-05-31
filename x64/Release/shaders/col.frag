#version 450 core
in vec3 normal;
in vec3 vertexColor;
in vec3 vertexPos;

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec3 normalOut;
layout(location = 2) out vec3 specOut;
layout(location = 3) out vec3 fragPos;

uniform vec3 pu_color;

void main()
{
    FragColor = vec4(pu_color, 1);
    fragPos = vertexPos;
    normalOut = normal;
    specOut = vec3(1);
	//FragColor = vec4(vertexColor, 1);
} 