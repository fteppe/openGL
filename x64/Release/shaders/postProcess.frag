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
	vec4 color = texture(color, UV);
	vec4 HDR = lightDim * color;
	//HDR = HDR/(HDR + vec4(1));
	ColorOutput = HDR;
}