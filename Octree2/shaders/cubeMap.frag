#version 430 core

in vec3 vertexPos;

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec3 normal;

uniform samplerCube skybox;

void main()
{
	
	vec3 pos = vec3(vertexPos.x,vertexPos.y,vertexPos.z);
	normal  = vec3(0);
    FragColor = texture(skybox , normalize(pos));
	//FragColor = vec4(vertexPos,1);
}
