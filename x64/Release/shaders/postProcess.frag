#version 430 core

in vec3 vertexPos;
in vec2 UV;

out vec4 ColorOutput;

uniform sampler2D color;
uniform sampler2D normals;
uniform sampler2D depth;
uniform sampler2D lights;

uniform float near;
uniform float far;

vec4 blur(sampler2D map, float initialOffset, int quality);
void makeOffsetMat(float offset, inout vec2[9] offsetMat);

void main()
{
    float depthVal = (texture(depth, UV).r);
    depthVal = clamp((depthVal - 0.90)*10, 0,1);
    //FragColor = vec4(depthVal);
    vec3 fogColor = vec3((depthVal)*vec4(1) + (1-depthVal)*texture(color,UV));
	
    //FragColor = vec4(fogColor,1);
	vec4 lightDim = texture(lights, UV);
	vec4 colorSample = texture(color, UV);
	vec4 HDR = lightDim * colorSample;
	HDR = HDR/(HDR + vec4(1));
	ColorOutput = HDR;

	//We are going to go for a blurry image post processing.
	// Building the offset matrix 
	float offset = 0.5f;

	
	offset = depthVal/100;
	clamp(offset, 0, 0.1);

	//This is a blur kernel;



	ColorOutput = blur(color, offset, 5);

}

vec4 blur(sampler2D map, float initialOffset, int quality)
{
	
	float kernel[9] = {
	2, 2, 2,
	2,0,2,
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
	offsetMat[0] = vec2(-offset, offset); //top left
	offsetMat[1] =	vec2(0, offset);							//top middle
	offsetMat[2] =	vec2(offset, offset);						// top right
	offsetMat[3] =	vec2(-offset, 0);							// middle right
	offsetMat[4] =	vec2(0,0);									//middle
	offsetMat[5] =	vec2(offset, 0);							//middle right
	offsetMat[6] =	vec2(-offset, -offset);						//bottom left
	offsetMat[7] =	vec2(0, -offset);							//bottom middle
	offsetMat[8] =	vec2(offset, -offset);						//bottom left
}