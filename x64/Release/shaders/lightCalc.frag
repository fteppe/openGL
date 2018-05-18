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
	//intensity = max(dot(normal, lightDir),0) * intensity * inversesqrt(lightDistance);

    //This is based on the incidance of the light.
    float lightFlux = max(dot(normal, lightDir),0);
    //We choose another function for attenuation:
    float paramA = 0.1;
    float paramB = 0.01;
    float attenuation = 1.0/(1.0 + paramA * lightDistance + paramB * lightDistance * lightDistance);


	vec3 intensityVec = vec3(intensity * lightFlux * attenuation) * lightCol;
	return intensityVec;
}

float shadowCalculation(vec4 fragShadowPos, sampler2D shadowMap)
{
	vec3 shadowPos = fragShadowPos.xyz/fragShadowPos.w;
	shadowPos = shadowPos * 0.5 + 0.5;
    float shadowTex =  texture(shadowMap, shadowPos.xy).r ;
    float currentDepth = (shadowPos.z);
    
    float bias = 0.005;
	//So if the depth is in front of the closest item to the light then it's in the light.
    float outVal  = currentDepth - bias > shadowTex  ? 0.0 : 1.0;

	//This is to know if what is outside the shadowmap is. We consider that it's in the shadow.
	outVal = (shadowPos.x>1.0 || shadowPos.x<0.0) ? 0.0 : outVal;
	outVal = (shadowPos.y>1.0 || shadowPos.y<0.0) ? 0.0 : outVal;

	return outVal;
}