#version 330 core
in vec3 normal;
in vec3 vertexColor;
in vec3 vertexPos;
in vec2 UV;
out vec4 FragColor;

uniform float[7] light;
//uniform sampler2D diffuse;

void main()
{
	

	//vec3 lightCol = vec3(light[3],light[4],light[5]);
	//vec3 nothing = lightCol;

	//vec3 intensityVec = vec3(1,1,1);//* lightCol;
	//we add a constant value to the intensity, so it is never dark.
	//intensityVec = intensityVec + vec3(0.1,0.1,0.1);
	//vec3 color = vec3(0.5,0.5,0.5);//-normal.xy,normal.z);
    FragColor = vec4(1.0);
	//FragColor = vec4(vertexColor, 1);
} 