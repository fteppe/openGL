 #version 430 core
in vec3 normalWorld;
in vec3 tangentWorld;
in vec3 biTangentWorld;
in vec3 vertexColor;
in vec3 fragPosWorld;
in vec2 UV;
in vec3 posTan;
in vec3 camTan;
//in vec3 tang;
 
in vec4 fragDepthShadow;

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec3 normalOut;
layout(location = 2) out vec3 specOut;
layout(location = 3) out vec3 fragPos;
layout(location = 4) out float fragDepthShadowOut;

uniform float[7] light;
uniform float time;
uniform vec3 camPos;
uniform sampler2D diffuse;
uniform sampler2D specularityMap;
uniform sampler2D normalMap;
uniform sampler2D depthMap;
uniform samplerCube skybox;

uniform sampler2D shadowMap;

vec3 fragLight(float light[7], vec3 normalWorld, vec3 fragPosWorld);
vec3 specCalc(float light[7], vec3 normalWorld, vec3 fragPosWorld, vec3 camPos, float specPow, float specVal);
vec3 albedo(vec2 UV);
vec3 normalValue(vec3 normal, vec3 tangent, vec3 bitTangent,vec2 UVin);
vec2 parralax(vec3 camTan, vec3 posTan);
float water();
vec4 cubeMapReflection(vec3 normalWorld, vec3 fragPosWorld, vec3 camPos);
float shadowCalculation(vec4 fragShadowPos, sampler2D shadowMap);


float shadowCalculation(vec4 fragShadowPos, sampler2D shadowMap)
{
	vec3 shadowPos = fragShadowPos.xyz/fragShadowPos.w;
	shadowPos = shadowPos * 0.5 + 0.5;
    float shadowTex =  texture(shadowMap, shadowPos.xy).r ;
    float currentDepth = (shadowPos.z);
    
    float bias = 0.005;
    float outVal  = currentDepth - bias > shadowTex  ? 0.0 : 1.0;
	outVal = (shadowPos.x>1.0 || shadowPos.x<0.0) ? 1.0 : outVal;
	outVal = (shadowPos.y>1.0 || shadowPos.y<0.0) ? 1.0 : outVal;

	return outVal;
}

void main()
{
   
    
    fragDepthShadowOut = shadowCalculation(fragDepthShadow, shadowMap);
	vec3 pos = fragPosWorld;
	vec3 normal_ = vec3(0,1,0);//normalWorld;
	vec2 translation = parralax(camTan, posTan);
	vec2 newUV = UV + translation;
	fragPos = pos;

	vec3 bumpVal = texture(normalMap, UV).rgb;
	normal_ = normalValue(normalWorld, tangentWorld, biTangentWorld, newUV);
	
	vec4 specularity = vec4(texture(specularityMap, newUV));
	float specVal = (specularity.r);
	specVal = 1  -specVal;

	float specPow = 32;
	//We use the spec map as a bump map as well, to make it look a bit better
	//we add a constant value to the intensity, so it is never dark.
	vec3 ambiant = vec3(0);
	vec3 intensityVec = fragLight(light, normal_, pos);
	vec3 specVec = specCalc(light, normal_, pos, camPos, specPow, specVal);
	vec4 color = vec4(albedo(newUV),1);
	//color *=  + specVec + ambiant;
	color = color;
	
    //FragColor = color;
	
	specOut = vec3(specVal,0,0);
	//the output of the normal vector must fit in [0,1]
	normalOut = normal_/2 + vec3(0.5);
	vec4 reflectionVal = cubeMapReflection(normal_, fragPosWorld, camPos);
	FragColor =  color;
}

vec3 albedo(vec2 UVin)
{
	vec3 col = vec3(texture(diffuse, UVin));
	return col;
}

vec2 parralax(vec3 camTan, vec3 posTan)
{
	float heightScale = 0.1;
	//If there is no bound map we get out of the function without trying to give a valid result.
	if(textureSize(depthMap,0).x < 2)
	{
		return vec2(0);
	}
	
	int nbSample = 200;

	float nbSamplef = float(nbSample);
	float stepSize = 1/ nbSamplef;

	vec3 viewDir = normalize( posTan - camTan );
	float height =  texture(depthMap, UV).r;    
	
	float currentHeight = 1;
	//this vector will go through the layers.
	vec2 stepVector = stepSize * heightScale * viewDir.xy;
	vec2 v = stepVector;
	while( currentHeight > height ) 
	{
		v = v + stepVector;
		currentHeight = currentHeight - stepSize;
		height = texture(depthMap, UV + v).r;
	}
	float previousHeight = texture(depthMap, UV + v - stepVector).r;
	float delta1 = height - currentHeight;
	float delta2 = currentHeight + stepSize - previousHeight;

	float weight = delta1 / (delta1+delta2); 
	//interpolation
	return v.xy  - (weight) * stepVector ;
}

float water()
{
	return (abs((sin(time * 4 + fragPosWorld.x * 20) +  2 * sin( - time  + fragPosWorld.x*10 + 5) +  sin( - time  + fragPosWorld.z*10 + 5))/50 ));
}

vec3 normalValue(vec3 normal, vec3 tangent, vec3 biTangent,vec2 UVin)
{
	vec3 normalMapVal = vec3(texture(normalMap, UVin));
	vec3 baseLine = vec3(0.5); //since colors go from 0-1. a vector that is null has for value 0.5.
	normalMapVal = normalMapVal - baseLine;
	//We substract the Y value of the normal map because it seems that my map has the +y vector in the -y direction of the UV vector.
	return normalize(normalMapVal.z*normalize(normal) + normalMapVal.x*normalize(tangent) - normalMapVal.y*normalize(biTangent));
}

//we put the cos value in the last part of the vec4.
vec4 cubeMapReflection(vec3 normalWorld, vec3 fragPosWorld, vec3 camPos)
{
	
	vec3 camDir = normalize(fragPosWorld - camPos);
	vec3 lightReflection = reflect( camDir, normalWorld);
	float cosVal = max(dot(camDir, lightReflection),0);
	//return vec4(reflect( camDir, normalWorld), 0);
	return vec4(vec3(texture(skybox, lightReflection)), cosVal);
}