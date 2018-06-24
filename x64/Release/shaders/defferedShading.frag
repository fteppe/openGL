#version 430 core

#include "lightStruct.hfrag"
#include "specularCalc.frag"
#include "lightCalc.frag"

in vec3 vertexPos;
in vec2 UV;

layout(location = 0) out vec4 ColorOutput;
layout(location = 1) out vec4 bright;

uniform sampler2D color;
uniform sampler2D normals;
uniform sampler2D depth;
uniform sampler2D specularity;
uniform sampler2D fragPos;
uniform sampler2D shadowMaps[5];
uniform samplerCube skybox;


uniform float near;
uniform float far;
uniform mat4 inverseCam;
uniform vec3 camPos;
uniform Light lights[10];
uniform int numLights;
uniform mat4 shadowProjection[5];

vec4 blur(sampler2D map, float initialOffset, int quality);
void makeOffsetMat(float offset, inout vec2[9] offsetMat);
float linearizeDepth(float depth, float near, float far);
vec3 getPositionFromDepth(sampler2D map, vec2 screenPos, mat4 inverseCam);
vec3 valLight();

void main()
{
	vec3 light = valLight();
    float depthVal = (texture(depth, UV)).r;
    depthVal = linearizeDepth(depthVal, 0.1, 200);

	float centerDepth = texture(depth, vec2(0.5, 0.5)).r;
	centerDepth = linearizeDepth(centerDepth, 0.1, 200);
    //FragColor = vec4(depthVal);
    vec3 fogColor = vec3((depthVal)*vec4(1) + (1-depthVal)*texture(color,UV));
	
    //FragColor = vec4(fogColor,1);
	vec3 normal = texture(normals, UV).rgb;
	vec4 lightDim = texture(specularity, UV);
	vec4 colorSample = texture(color, UV);
	vec4 IBL = texture(skybox, normal , 6);
	IBL = pow(IBL + 0.2, vec4(10));
	//colorSample = vec4(1);
	colorSample = colorSample * (vec4(light,0));
	ColorOutput = colorSample;
	
	//ColorOutput = vec4(normal, 1);
	float bloomThreshold = 10.0;
	//The numbers in the brightness calculations are based on human perception.
	float brightness = dot(colorSample.rgb, vec3(0.2126, 0.7152, 0.0722));
	float isABoveThreshold = float(brightness > bloomThreshold);
	bright = colorSample * isABoveThreshold;
	//bright = vec4(0.0);
	
    vec3 pos = vec3(texture(fragPos, UV));
}





float linearizeDepth(float depth, float near, float far)
{
	float z = depth;
	return (depth)/ (far + near -depth *(far - near));

}


//vec3 diffuseCalc(Light light, vec3 normal, vec3 vertexPos);

vec3 specCalc(Light light, vec3 normal, vec3 fragPos, vec3 camPos, float specPow, float specVal);

vec3 valLight()
{
	float ambiant = 0.06f; 
	vec3 intensityVec = vec3(0);
	for(int i = 0; i < numLights; i++)
	{
        vec3 lightIntensity = vec3(0);
		vec3 normal_ = vec3(texture(normals, UV));
		vec3 pos = vec3(texture(fragPos, UV));
		Light light_ = lights[i];
		lightIntensity = lightIntensity + diffuseCalc(light_, normal_, pos);
		vec2 specValues = texture(specularity, UV).rg;
        float specVal = clamp(specValues.r, 0, 10);
        float glossyness = clamp(specValues.g, 16 , 4096);
		vec3 cameraPosition = camPos;
		lightIntensity = lightIntensity + specCalc(light_, normal_, pos, cameraPosition, glossyness, specVal);

        int shadowIndex = light_.shadowIndex;
        if(shadowIndex >= 0)
        {
            vec4 shadowPos = shadowProjection[shadowIndex] * vec4(pos,1);
            float shadow = shadowCalculation(shadowPos, shadowMaps[shadowIndex]);
            lightIntensity *= vec3(shadow);
        }
        
        intensityVec = intensityVec + lightIntensity;
	}
	return intensityVec + ambiant;
}

