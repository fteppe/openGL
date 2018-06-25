#version 430 core

#include "sampleEquirectangular.glsl"

in vec3 normal;
in vec3 vertexColor;
in vec3 vertexPos;

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec3 normalOut;
layout(location = 2) out vec3 specOut;
layout(location = 3) out vec3 fragPos;

uniform vec3 pu_color;
uniform vec3 pu_spec;


void main()
{

    FragColor = vec4(pu_color, 1);
    fragPos = vertexPos;
    normalOut = normal;
    specOut = pu_spec;
	//FragColor = vec4(vertexColor, 1);
} 