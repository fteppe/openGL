#version 430 core
#include "lightStruct.hfrag"
#include "lightCalc.frag"

vec3 fragLight(float light[7], vec3 normal, vec3 vertexPos);

vec3 specCalcFull(float light[7], vec3 normal, vec3 vertexPos, vec3 camPos)
{
	return vec3(0.0);
}
vec3 specCalc(Light light, vec3 normal, vec3 fragPos, vec3 camPos, float specPow, float specVal)
{
	vec3 eyeVec = normalize(camPos - fragPos);
	//we get the value of the light energy hitting the fragment
	vec3 lightPos = light.pos;
	vec3 lightDir = normalize(fragPos - lightPos);
	normal = normalize(normal);
	float lightDirZ = dot(-lightDir, normal);
	vec3 lightReflection = reflect(lightDir,normal);
	//specPow = 20;
	float cosVal = max(dot(eyeVec, lightReflection),0);
	float specIntensity = specVal * pow(cosVal, specPow);
	vec3 returnVal = vec3( specIntensity  ) * light.color;
	//returnVal = vec3( cosVal  ) * light.color;
	return returnVal;
}