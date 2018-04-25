#version 430 core

uniform sampler2D specularMap;

vec3 fragLight(float light[7], vec3 normal, vec3 vertexPos);

vec3 specCalcFull(float light[7], vec3 normal, vec3 vertexPos, vec3 camPos)
{
	return vec3(0.0);
}
vec3 specCalc(float light[7], vec3 normal, vec3 fragPos, vec3 camPos, float specPow, float specVal)
{
	vec3 eyeVec = normalize(camPos - fragPos);
	//we get the value of the light energy hitting the fragment
	vec3 lightIntensity = fragLight(light, normal, fragPos);
	vec3 lightPos = vec3(light[0], light[1], light[2]);
	vec3 lightDir = normalize(fragPos - lightPos);
	normal = normalize(normal);
	float lightDirZ = dot(-lightDir, normal);
	vec3 lightReflection = reflect(lightDir,normal);

	float cosVal = max(dot(eyeVec, lightReflection),0);
	float specIntensity = specVal * pow(cosVal, specPow);
	vec3 returnVal = vec3( specIntensity  );
	return returnVal;
}