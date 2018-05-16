#version 430 core

#include "lightStruct.hfrag"
#include "specularCalc.frag"
#include "lightCalc.frag"

in vec3 vertexPos;
in vec2 UV;

out vec4 ColorOutput;

uniform sampler2D color;
uniform sampler2D normals;
uniform sampler2D depth;
uniform sampler2D specularity;
uniform sampler2D fragPos;
uniform sampler2D shadowDistance;


uniform float near;
uniform float far;
uniform mat4 inverseCam;
uniform vec3 camPos;
uniform Light lights[10];
uniform int numLights;

vec4 blur(sampler2D map, float initialOffset, int quality);
void makeOffsetMat(float offset, inout vec2[9] offsetMat);
float linearizeDepth(float depth, float near, float far);
vec3 getPositionFromDepth(sampler2D map, vec2 screenPos, mat4 inverseCam);
vec3 valLight();

void main()
{
	vec3 light = valLight();
    float depthVal = (texture(depth, UV)).r;
    depthVal = linearizeDepth(depthVal, 0.1, 200);

	float centerDepth = texture(depth, vec2(0.5, 0.5)).r;
	centerDepth = linearizeDepth(centerDepth, 0.1, 200);
    //FragColor = vec4(depthVal);
    vec3 fogColor = vec3((depthVal)*vec4(1) + (1-depthVal)*texture(color,UV));
	
    //FragColor = vec4(fogColor,1);
	vec4 lightDim = texture(specularity, UV);
	vec4 colorSample = texture(color, UV);
	colorSample = colorSample * vec4(light,0);
	vec4 HDR = colorSample;
	HDR = HDR/(HDR + vec4(1));
	ColorOutput = HDR;
	//ColorOutput = texture(normals, UV);
	//We are going to go for a blurry image post processing.
	// Building the offset matrix 
	float offset = 0.5f;

	
	float faroffset = (depthVal - pow(2, 10*centerDepth));
	//faroffset = clamp(faroffset, 0, 2000);
	float closeOffset = (0.02+centerDepth/100  - depthVal);
	//closeOffset = clamp(closeOffset, 0, 2000);

	offset = max(closeOffset, faroffset)/100;
	
	offset = clamp (offset, 0.0, 0.1);
	//ColorOutput = blur(color, offset, 5) * (blur(shadowDistance, 0.0, 1).r+0.1);
    //ColorOutput = texture(specularity, UV);
	//float specVal = texture(specularity, UV).r;
}

vec4 blur(sampler2D map, float initialOffset, int quality)
{
	
	float kernel[9] = {
	2,2,2,
	2,2,2,
	2,2,2};

	float totalKernel = 0;
	for(int  i=0 ; i < kernel.length(); i++)
	{
		totalKernel += kernel[i];
	}


	vec4 outputVal = vec4(0);
	float offset = initialOffset;
	offset = offset + offset/quality;
	for(int i = 0; i< quality; i++)
	{
		vec2 offsetMat[9];
		makeOffsetMat(offset, offsetMat);
		for(int i = 0; i < 9; i++)
		{
			vec2 offsetUV = vec2 (UV + offsetMat[i]);
			outputVal += (texture(map, offsetUV) * (kernel[i]/(quality * totalKernel)));
		}
		offset = offset - offset/quality;
	}
	return outputVal;
}

void makeOffsetMat(float offset, inout vec2[9] offsetMat)
{
	offsetMat[0] = vec2(-offset, offset);						//top left
	offsetMat[1] =	vec2(0, offset);							//top middle
	offsetMat[2] =	vec2(offset, offset);						// top right
	offsetMat[3] =	vec2(-offset, 0);							// middle right
	offsetMat[4] =	vec2(0,0);									//middle
	offsetMat[5] =	vec2(offset, 0);							//middle right
	offsetMat[6] =	vec2(-offset, -offset);						//bottom left
	offsetMat[7] =	vec2(0, -offset);							//bottom middle
	offsetMat[8] =	vec2(offset, -offset);						//bottom left
}

float linearizeDepth(float depth, float near, float far)
{
	float z = depth;
	return (depth)/ (far + near -depth *(far - near));

}

vec3 getPositionFromDepth(sampler2D map, vec2 screenPos, mat4 inverseCam)
{
	float depth = texture(map, screenPos).r * 2.0 - 1.0;
	//this is because the UV is from the corner and we want the position with the origin in the center.
	vec2 adjustedScreenPos = screenPos * 2.0 - 1.0;
	vec4 pos = vec4(adjustedScreenPos , depth, 1.0);

	pos /= pos.w;
	return vec3(inverseCam * pos);
}

//vec3 diffuseCalc(Light light, vec3 normal, vec3 vertexPos);

vec3 specCalc(Light light, vec3 normal, vec3 fragPos, vec3 camPos, float specPow, float specVal);

vec3 valLight()
{
	float ambiant = 0.1f; 
	vec3 intensityVec = vec3(0);
	for(int i = 0; i < numLights; i++)
	{
		vec3 normal_ = vec3(texture(normals, UV));
		vec3 pos = vec3(texture(fragPos, UV));
		Light light_ = lights[i];
		intensityVec = intensityVec + diffuseCalc(light_, normal_, pos);
		float specPow = 64;
		float specVal = texture(specularity, UV).r;
		vec3 cameraPosition = camPos;
		intensityVec = intensityVec + specCalc(light_, normal_, pos, cameraPosition, specPow, specVal);
	}
	return intensityVec + ambiant;
}