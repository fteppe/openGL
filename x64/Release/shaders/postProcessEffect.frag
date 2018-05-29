#version 430 core

#include "kernel.frag"

in vec2 UV;

out vec4 colorOut;

uniform sampler2D fullColor;
uniform sampler2D bright;
uniform sampler2D depth;


vec4 depthOfField(sampler2D map,sampler2D depth, float depthVal, float depthThreshold, int quality);
vec4 blur(sampler2D map, float initialOffset, int quality);
vec4 toneMapping(vec4 value);

void main(){
    

    float depthVal = texture(depth, UV).r;
    float depthThreshold = texture(depth, vec2(0.5,0.5)).r + 0.1;
    vec4 color = depthOfField(fullColor, depth, depthVal, depthThreshold, 4);
	vec4 bloom = blur(bright, 0.01, 4);
	//bloom = vec4(color);
    colorOut = toneMapping((color)/2);
	//colorOut = bloom;
}

vec4 blur(sampler2D map, float initialOffset, int quality)
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
			vec2 offsetUV = vec2 (UV + offsetMat[j]);
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