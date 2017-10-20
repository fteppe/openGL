#version 330 core
in vec3 normal;
in vec3 vertexColor;
in vec3 vertexPos;
in vec2 UV;
out vec4 FragColor;

uniform float[7] light;
uniform vec3 camPos;
uniform sampler2D diffuse;

vec3 fragLight(float light[7], vec3 normal, vec3 vertexPos);
vec3 specCalc(float light[7], vec3 normal, vec3 vertexPos, vec3 campPos, float specVal);

void main()
{
	//we add a constant value to the intensity, so it is never dark.
	vec3 intensityVec = fragLight(light, normal, vertexPos) + vec3(0.1);
	vec3 specVec = specCalc(light, normal, vertexPos, camPos, 256);
	vec4 color = vec4(0.5);//vec4(texture(diffuse, UV));
    FragColor = vec4(intensityVec * specVec,1) * color ;
}