#version 430 core

#include "lightStruct.hfrag"


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

vec3 diffuseCalc(Light light, vec3 normal, vec3 vertexPos)
{
	vec3 lightPos = light.pos;
	vec3 lightCol = light.color;
	float intensity = light.intensity;
	float lightDistance = length(lightPos - vertexPos);
	vec3 lightDir = normalize(lightPos - vertexPos);
	//The light intensity is the dot product of the normal and the light direction floored at one. We also take into account the intensity of the light and it's distance.
	intensity = max(dot(normal, lightDir),0) * intensity * inversesqrt(lightDistance);
	vec3 intensityVec = vec3(intensity, intensity, intensity) * lightCol;
	return intensityVec;
}

