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
 

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec3 normalOut;
layout(location = 2) out vec3 specOut;
layout(location = 3) out vec3 fragPos;

uniform float time;
uniform vec3 camPos;
uniform sampler2D diffuse;
uniform sampler2D specularityMap;
uniform sampler2D normalMap;
uniform sampler2D depthMap;
uniform samplerCube skybox;
uniform vec3 pu_specIn;
uniform sampler2D shadowMap;

uniform float pu_depth;
uniform vec3 pu_col;
uniform float pu_Normalstrength;

vec3 fragLight(float light[7], vec3 normalWorld, vec3 fragPosWorld);
vec3 albedo(vec2 UV);
vec3 normalValue(vec3 normal, vec3 tangent, vec3 bitTangent,vec2 UVin, float strength);
vec2 parralax(vec3 camTan, vec3 posTan);
float water();
vec4 cubeMapReflection(vec3 normalWorld, vec3 fragPosWorld, vec3 camPos);
float shadowCalculation(vec4 fragShadowPos, sampler2D shadowMap);


void main()
{
   
    
	vec3 pos = fragPosWorld;
	vec3 normal_ = vec3(0,1,0);//normalWorld;
	vec2 translation = parralax(camTan, posTan);
	vec2 newUV = UV + translation;
    newUV = vec2(newUV.x , 1 - newUV.y);

	vec3 bumpVal = texture(normalMap, UV).rgb;
	normal_ = normalValue(normalWorld, tangentWorld, biTangentWorld, newUV, pu_Normalstrength);

	//We move the position so that we get the position of the fragment after parallax.
    pos =  pos + translation.x * tangentWorld + translation.y * biTangentWorld;
	fragPos = pos;
	
	float specPow = 32;
	//We use the spec map as a bump map as well, to make it look a bit better
	//we add a constant value to the intensity, so it is never dark.
	vec3 ambiant = vec3(0);
	vec4 color = vec4(pu_col,1);
	if(length(pu_col) < 0.1)
	{
		color = vec4(albedo(newUV),1);
	}
	else
	{
		
	}
	//color = vec4(newUV, 0, 0);

	normalOut = normal_;
	vec4 reflectionVal = cubeMapReflection(normal_, fragPosWorld, camPos);
	FragColor =  color;
	specOut = pu_specIn;
}

vec3 albedo(vec2 UVin)
{
	vec3 col = vec3(texture(diffuse, UVin));
	return col;
}

vec2 parralax(vec3 camTan, vec3 posTan)
{
	float heightScale = pu_depth;
	//If there is no bound map we get out of the function without trying to give a valid result.
	if(textureSize(depthMap,0).x < 2 || pu_depth < 0.001)
	{
		return vec2(0);
	}
	
	int nbSample = 30;

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

vec3 normalValue(vec3 normal, vec3 tangent, vec3 biTangent,vec2 UVin, float strength)
{
	vec3 normalMapVal = vec3(texture(normalMap, UVin));
	vec3 newNormal = normal;
	if(length(normalMapVal) > 0.5)
	{
			//normalMapVal = normalMapVal  *strength;
		vec3 baseLine = vec3(0.5); //since colors go from 0-1. a vector that is null has for value 0.5.
		normalMapVal = normalMapVal - baseLine;
		//We substract the Y value of the normal map because it seems that my map has the +y vector in the -y direction of the UV vector.
		vec3 returnVal = normalize(normalMapVal.z*normalize(normal) + normalMapVal.x*normalize(tangent) - normalMapVal.y*normalize(biTangent));
		newNormal = max(returnVal, normal);
	}

	return newNormal;
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