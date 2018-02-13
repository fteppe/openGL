#version 330 core
in vec3 normal;
in vec3 vertexColor;
in vec3 vertexPos;
in vec2 UV;
in vec3 posTan;
in vec3 camTan;
in vec3 tang;
 
out vec4 FragColor;

uniform float[7] light;
uniform vec3 camPos;
uniform sampler2D diffuse;
uniform sampler2D spec;
uniform sampler2D bump;

void main()
{
	

	//vec3 lightCol = vec3(light[3],light[4],light[5]);
	//vec3 nothing = lightCol;

	//vec3 intensityVec = vec3(1,1,1);//* lightCol;
	//we add a constant value to the intensity, so it is never dark.
	//intensityVec = intensityVec + vec3(0.1,0.1,0.1);
	//vec3 color = vec3(0.5,0.5,0.5);//-normal.xy,normal.z);
	vec2 dir = normalize(posTan.xy - camTan.xy);
    //FragColor = texture(diffuse, UV);//vec4(UV,0,1);
	FragColor = vec4(UV,0, 1);
} 