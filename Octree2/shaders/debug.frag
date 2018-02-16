 #version 330 core
in vec3 normalWorld;
in vec3 tangentWorld;
in vec3 biTangentWorld;
in vec3 vertexColor;
in vec3 fragPosWorld;
in vec2 UV;
in vec3 posTan;
in vec3 camTan;
//in vec3 tang;
 
out vec4 FragColor;

uniform float[7] light;
uniform float time;
uniform vec3 camPos;
uniform sampler2D diffuse;
uniform sampler2D spec;
uniform sampler2D normalMap;
uniform sampler2D depthMap;
//uniform sampler2D reflectionTex;

vec3 normalValue(vec3 normal, vec3 tangent, vec3 biTangent,vec2 UVin);
void main()
{
	vec3 normalMapVal = vec3(texture(normalMap, UV));
	vec3 normalVal = normalValue(normalWorld, tangentWorld, biTangentWorld, UV);
	FragColor = vec4(normalVal, 1);
} 

vec3 normalValue(vec3 normal, vec3 tangent, vec3 biTangent,vec2 UVin)
{
	vec3 normalMapVal = vec3(texture(normalMap, UVin));
	vec3 baseLine = vec3(0.5); //since colors go from 0-1. a vector that is null has for value 0.5.
	normalMapVal = normalMapVal - baseLine;
	return normalize(normalMapVal.z*normalize(normal) + normalMapVal.x*normalize(tangent) + normalMapVal.y*normalize( biTangent));
}