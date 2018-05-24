#version 430 core

#include "kernel.frag"

in vec2 UV;

out vec4 colorOut;

uniform sampler2D fullColor;
uniform sampler2D bright;
uniform sampler2D depth;


vec4 blur(sampler2D map,sampler2D depth, float depthVal, float depthThreshold, int quality);

void main(){
    

    float depthVal = texture(depth, UV).r;
    float depthThreshold = texture(depth, vec2(0.5,0.5)).r + 0.1;
    vec4 color = blur(fullColor, depth, depthVal, depthThreshold, 4);
    colorOut = color;
}


vec4 blur(sampler2D map,sampler2D depth, float depthVal, float depthThreshold, int quality)
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

    outputVal = outputVal;
	return outputVal;
}