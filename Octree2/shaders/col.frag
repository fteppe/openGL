#version 330 core
in vec3 normal;
in vec3 vertexColor;
in vec3 vertexPos;

out vec4 FragColor;

uniform float[7] light;

void main()
{

	vec3 lightPos = vec3(light[0], light[1], light[2]);
	vec3 lightCol = vec3(light[3],light[4],light[5]);
	float intensity = light[6];
	float lightDistance = length(lightPos - vertexPos);
	vec3 lightDir = normalize(lightPos - vertexPos);
	//The light intensity is the dot product of the normal and the light direction floored at one. We also take into account the intensity of the light and it's distance.
	intensity = max(dot(normal, lightDir),0) * intensity * inversesqrt(lightDistance);
	vec3 intensityVec = vec3(intensity, intensity, intensity) * lightCol;

	vec3 color = vec3(-normal.xy,normal.z);
    FragColor = vec4(intensityVec * color,1);
	//FragColor = vec4(normal * vec3(1,1,1), 1);
} 