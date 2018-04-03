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
	float offset = 0.005f;

	
	offset = depthVal/100;
	clamp(offset, 0, 0.05);
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

	//This is a blur kernel;
	
	float kernel[9] = {
	1.0/16 ,1.0/8, 1.0/16,
	1.0/8, 4.0/16, 1.0/8,
	1.0/16,1.0/8,1.0/16,};

	ColorOutput = vec4(0);
	//We then apply the kernel
	for(int i = 0; i < 9; i++)
	{
		vec2 offsetUV = vec2 (UV + offsetMat[i]);
		ColorOutput += (texture(color, offsetUV) * kernel[i]);
	}
	//ColorOutput = texture(lights, UV);

}