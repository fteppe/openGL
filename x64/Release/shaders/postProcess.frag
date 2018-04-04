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

	
	offset = depthVal/50;
	clamp(offset, 0, 0.5);
	/*
	vec2 offsetMat[9] = {vec2(-offset, offset), //top left
	vec2(0, offset),							//top middle
	vec2(offset, offset),						// top right
	vec2(-offset, 0),							// middle right
	vec2(0,0),									//middle
	vec2(offset, 0),							//middle right
	vec2(-offset, -offset),						//bottom left
	vec2(0, -offset),							//bottom middle
	vec2(offset, -offset)						//bottom left
	};
	*/

	//This is a blur kernel;
	
	float kernel[9] = {
	1.0/16 ,1.0/8, 1.0/16,
	1.0/8, 4.0/16, 1.0/8,
	1.0/16,1.0/8,1.0/16,};

	ColorOutput = vec4(0);
	//We then apply the kernel
	/*
	for(int i = 0; i < 9; i++)
	{
		vec2 offsetUV = vec2 (UV + offsetMat[i]);
		ColorOutput += (texture(color, offsetUV) * kernel[i]);
	}
	*/

	vec2 offsetMat[9];
	//makeOffsetMat(offset, offsetMat);
	//ColorOutput = vec4(offsetMat[0],0,0);
	ColorOutput = blur(color, offset, 10);

}

vec4 blur(sampler2D map, float initialOffset, int quality)
{
	
	float kernel[9] = {
	1.0/(16*quality) ,1.0/(8*quality), 1.0/(16*quality),
	1.0/(8*quality), 4.0/(16*quality), 1.0/(8*quality),
	1.0/(16*quality),1.0/(8*quality),1.0/(16*quality),};

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
			outputVal += (texture(map, offsetUV) * kernel[i]);
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