#version 330 core
in vec3 normal;
in vec3 vertexColor;
in vec3 vertexPos;
in vec2 UV;
out vec4 FragColor;

uniform float[7] light;
uniform sampler2D diffuse;

vec3 fragLight(float light[7], vec3 normal, vec3 vertexPos);

void main()
{


	//It doesn't matter we just want to write to the Z-buffer.
    FragColor = vec4(1) ;


}
