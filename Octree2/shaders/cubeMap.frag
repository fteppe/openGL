#version 330 core

in vec3 vertexPos;

out vec4 FragColor;

uniform samplerCube skybox;

void main()
{


	//we add a constant value to the intensity, so it is never dark.
	vec3 pos = vec3(vertexPos.x,vertexPos.y,vertexPos.z);
    FragColor = texture(skybox , normalize(pos));
	//FragColor = vec4(vertexPos,1);


}
