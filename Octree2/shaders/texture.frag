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


	//we add a constant value to the intensity, so it is never dark.
	vec3 intensityVec = fragLight(light, normal, vertexPos) + vec3(0.1,0.1,0.1);
	vec4 color = vec4(texture(diffuse, UV));
    FragColor = vec4(intensityVec,1) * color ;


} 

vec3 fragLight(float light[7], vec3 normal, vec3 vertexPos)
{
	vec3 lightPos = vec3(light[0], light[1], light[2]);
	vec3 lightCol = vec3(light[3],light[4],light[5]);
	float intensity = light[6];
	float lightDistance = length(lightPos - vertexPos);
	vec3 lightDir = normalize(lightPos - vertexPos);
	//The light intensity is the dot product of the normal and the light direction floored at one. We also take into account the intensity of the light and it's distance.
	intensity = max(dot(normal, lightDir),0) * intensity * inversesqrt(lightDistance);
	vec3 intensityVec = vec3(intensity, intensity, intensity) * lightCol;

	return intensityVec;
}