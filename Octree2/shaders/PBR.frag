 #version 330 core
in vec3 normal;
in vec3 vertexColor;
in vec3 vertexPos;
in vec2 UV;
in vec3 posTan;
in vec3 camTan;
//in vec3 tang;
 
out vec4 FragColor;

uniform float[7] light;
uniform vec3 camPos;
uniform sampler2D diffuse;
uniform sampler2D spec;
uniform sampler2D normalMap;
uniform sampler2D depthMap;


vec3 fragLight(float light[7], vec3 normal, vec3 vertexPos);
vec3 specCalc(float light[7], vec3 normal, vec3 vertexPos, vec3 camPos, float specPow, float specVal);
vec3 albedo(vec2 UV);
vec2 parralax(vec3 camTan, vec3 posTan);

void main()
{
	vec3 pos = vertexPos;
	vec3 normal_ = normal;
	vec2 translation = parralax(camTan, posTan);
	vec2 newUV = UV + translation;

	if(newUV.x > 1 ||newUV.x <0 ||newUV.y>1 || newUV.y<0)
	{
		discard;
	}
	vec3 bumpVal = texture(normalMap, newUV).rgb;
	normal_ = normalize(normal + bumpVal * 100);
	
	vec4 specularity = vec4(texture(depthMap, newUV));
	float specVal = (specularity.r);
	if(specVal > 0.1)
	{
		specVal = 0;
	}
	else
	{
		specVal = 1;
	}

	float specPow = 32;
	//We use the spec map as a bump map as well, to make it look a bit better
	//we add a constant value to the intensity, so it is never dark.
	vec3 ambiant = vec3(0.1);
	vec3 intensityVec = fragLight(light, normal_, pos);
	vec3 specVec = specCalc(light, normal_, pos, camPos, specPow, specVal);
	vec4 color = vec4(albedo(newUV),1);
	//vec4 color = vec4(vec3(1),1);
    FragColor = vec4(  intensityVec + specVec + ambiant,1) * color;
	//FragColor = vec4(1);
	
}

vec3 albedo(vec2 UVin)
{
	vec3 col = vec3(texture(diffuse, UVin));
	float height = vec3(texture(depthMap, UVin)).r;

	if(height >0.1)
	{
		col = vec3 (0.5,0.5,0) * (height) * 3;
		if(int(height*100) % 5 < 0.002)
		{
			col = vec3(0.2,0.2,0.1);
		}
	}
	else if (height < 0.1)
	{
		col = vec3(0.2,0.2,1);
	}
	return col;
}

vec2 parralax(vec3 camTan, vec3 posTan)
{
	float heightScale = 0.1;
	int nbSample = 200;

	float nbSamplef = float(nbSample);
	float stepSize = 1/ nbSamplef;

	vec3 viewDir = normalize( posTan - camTan );
	float height =  texture(depthMap, UV).r;    
    vec2 p = viewDir.xy / viewDir.z * (height * heightScale);
   // return - p;

	//lets try to make the real one.
	
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
	float previousHeight = texture(spec, UV + v - stepVector).r;
	float delta1 = height - currentHeight;
	float delta2 = currentHeight + stepSize - previousHeight;

	float weight = delta1 / (delta1+delta2); 
	//interpolation
	return v.xy  - (weight) * stepVector ;
}