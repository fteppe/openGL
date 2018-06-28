#version 430 core

#include "kernel.frag"

in vec2 UV;

out vec4 colorOut;

uniform sampler2D fullColor;
uniform sampler2D bright;
uniform sampler2D depth;
uniform sampler2D normals;
uniform sampler2D fragPos;
uniform sampler2D specularity;


uniform vec3 camPos;
uniform mat4 viewSpace;
uniform float near;
uniform float far;

vec4 depthOfField(sampler2D map,sampler2D depth, float depthVal, float depthThreshold, int quality);
vec4 blur(sampler2D map, vec2 UVin, float initialOffset, int quality);
vec4 toneMapping(vec4 value);
vec3 screenSpaceReflection(sampler2D depthMap, vec3 reflection, vec3 origin ,mat4 viewSpaceMatrix);



void main(){
    

    float depthVal = texture(depth, UV).r;
    float depthThreshold = texture(depth, vec2(0.5,0.5)).r + 0.1;
    vec4 color = depthOfField(fullColor, depth, depthVal, depthThreshold, 4);
	vec4 bloom = blur(bright, UV,  0.01, 4);
	color = (color + bloom)/2;
    vec3 pos = texture(fragPos, UV).rgb;
    vec3 normal = texture(normals, UV).rgb;

    vec3 reflection = reflect(pos - camPos, normal);
    reflection = normalize(reflection);
    float spec = texture(specularity, UV).r;

    vec4 reflectionColor = vec4(0);

    if(spec > 0.01)
    {
    vec3 reflectHit = screenSpaceReflection(depth, reflection,pos, viewSpace);
   
        if(reflectHit.x < 1 && reflectHit.y < 1 && reflectHit.x > 0 && reflectHit.y > 0)
        {
            reflectionColor = texture(fullColor, reflectHit.xy);
            //reflectionColor = blur(fullColor, reflectHit.xy , 0.01, 3);
            float attenuation = (spec) * (1 - length(UV - reflectHit.xy)) * ( 1- length(reflectHit . y));
            reflectionColor *= attenuation;
        }
    }

    color = color + reflectionColor;
    colorOut = toneMapping(color);
	
}

vec3 screenSpaceReflection(sampler2D depthMap, vec3 reflection, vec3 origin, mat4 viewSpaceMatrix)
{
    int steps = 100;
    vec3 moveVector = reflection / (steps/10);
	float multiplier = 2;
    vec3 currentPos = origin;
    //currentPos = currentPos + moveVector;
    bool continueRay = true;
    vec2 UVpos = UV;
	vec2 previousUVpos = UV;
    float depthSample;
    int i = 0;
    float linearDepth = 0;
	float previousDepth = texture(depthMap, UV).r;
	float previousSample = texture(depthMap, UV).r;
    for( i =0; i < steps && continueRay; i++)
    {
        vec4 viewPos = (viewSpaceMatrix * vec4(currentPos,1));
        viewPos = viewPos / viewPos.w ;
        UVpos = viewPos.xy * 0.5 +0.5;
        float depthRay = viewPos.z  * 0.5 + 0.5 ;

        depthSample = texture(depthMap, UVpos).r;
		float bias = abs(depthRay - previousDepth);
		//bias = 0.0005;
        if(depthSample < depthRay - bias)
        {
            continueRay = false;
			//We try to do some linear interpolation.
			float delta1 = depthSample - depthRay;
			float delta2 = previousDepth - previousSample;
			float weight = delta1 / (delta1+delta2);
			depthRay = mix(depthRay, previousDepth, weight);
			UVpos = mix(UVpos, previousUVpos, weight);

			if(abs(depthRay - depthSample) > bias * 5)
			{
				// it just means that it is out of bound and isn't a valid reflection hit.
				UVpos = vec2(2);
			}

        }
		previousDepth = depthRay;
		previousSample = depthSample;
		previousUVpos =  UVpos;
        currentPos = currentPos + moveVector;
		//moveVector *= 1.1;
    }
	return vec3(UVpos, 0);
    //return vec3(float(i)/steps,0, 0);
}

vec4 blur(sampler2D map, vec2 UVin, float initialOffset, int quality)
{
	float kernel[9] = {
	2,2,2,
	2,2,2,
	2,2,2};


		vec4 outputVal = vec4(0);
	float offset = initialOffset;
	for(int i = 0; i< quality; i++)
	{
		vec2 offsetMat[9];
		makeOffsetMat(offset, offsetMat);
        vec4 accumulation = vec4(0);
        float kernelValAccumulation = 0.0;
		for(int j = 0; j < 9; j++)
		{
			vec2 offsetUV = vec2 (UVin + offsetMat[j]);
            float depthValLocal = texture(depth, offsetUV).r;
            accumulation += texture(map, offsetUV) * kernel[j];
            kernelValAccumulation += kernel[j];
		}

        outputVal = (outputVal *i)/(i+1)+ (accumulation ) / (kernelValAccumulation * (i+1));
		offset = offset - offset/quality;
	}

	return outputVal;
	return vec4(0);

}

vec4 depthOfField(sampler2D map,sampler2D depth, float depthVal, float depthThreshold, int quality)
{
	float kernel[9] = {
	2,2,2,
	2,2,2,
	2,2,2};
    float initialOffset = 0;
    if(depthVal > depthThreshold)
    {
        float weight = (depthVal - depthThreshold) *10;
        initialOffset = mix(0.0, 0.01, weight);
        initialOffset = clamp(initialOffset, 0.0, 0.01);
    }

	vec4 outputVal = vec4(0);
	float offset = initialOffset;
	for(int i = 0; i< quality; i++)
	{
		vec2 offsetMat[9];
		makeOffsetMat(offset, offsetMat);
        vec4 accumulation = vec4(0);
        float kernelValAccumulation = 0.0;
		for(int j = 0; j < 9; j++)
		{
			vec2 offsetUV = vec2 (UV + offsetMat[j]);
            float depthValLocal = texture(depth, offsetUV).r;
            if(depthValLocal >= depthVal)
            {
                accumulation += texture(map, offsetUV) * kernel[j];
                kernelValAccumulation += kernel[j];
            }
		}

        outputVal = outputVal/2+ (accumulation ) / (kernelValAccumulation * (2));
		offset = offset - offset/quality;
	}

	return outputVal;
}

vec4 toneMapping(vec4 value)
{
	vec4 toneMapped = (value / (vec4(1)+ value));
	float gamma = 2.2;
	toneMapped = pow(toneMapped, vec4(1.0 / gamma));
	return toneMapped;
}

