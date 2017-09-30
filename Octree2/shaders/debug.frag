#version 330 core
in vec3 normal;
in vec3 vertexColor;
in vec3 vertexPos;
in vec2 UV;
out vec4 FragColor;

uniform float[7] light;
uniform sampler2D diffuse;

void main()
{
	
	FragColor = vec4(texture(diffuse, UV));
	//FragColor = vec4(abs(normal),1);
	//FragColor = vec4(UV,0,0);
	//FragColor = vec4(vertexColor,1);// * vec4(texture(diffuse, UV));
} 