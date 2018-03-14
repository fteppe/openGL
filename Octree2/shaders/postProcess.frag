#version 430 core

in vec3 vertexPos;
in vec2 UV;

out vec4 FragColor;

uniform sampler2D color;
uniform sampler2D normals;
uniform sampler2D depth;

uniform float near;
uniform float far;

void main()
{
    float depthVal = (texture(depth, UV).r);
    depthVal = clamp((depthVal - 0.90)*10, 0,1);
    //FragColor = vec4(depthVal);
    vec3 fogColor = vec3((depthVal)*vec4(1) + (1-depthVal)*texture(color,UV));
	
    //FragColor = vec4(fogColor,1);
	FragColor = texture(color, UV);
}