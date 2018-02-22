#version 450 core

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
    depthVal = (depthVal - 0.98)*100;
    FragColor = vec4(depthVal);
    //FragColor = texture(depth,UV);
}