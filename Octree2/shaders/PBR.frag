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
uniform sampler2D bump;


vec3 fragLight(float light[7], vec3 normal, vec3 vertexPos);
vec3 specCalc(float light[7], vec3 normal, vec3 vertexPos, vec3 camPos, float specPow, float specVal);
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
	vec3 bumpVal = texture(bump, newUV).rgb;
	normal_ = normalize(normal + bumpVal);
	
	vec4 specularity = vec4(texture(spec, newUV));
	float specVal = specularity.r * 10;
	float specPow = 32;
	//We use the spec map as a bump map as well, to make it look a bit better
	//we add a constant value to the intensity, so it is never dark.
	vec3 ambiant = vec3(0.5);
	vec3 intensityVec = fragLight(light, normal_, pos);
	vec3 specVec = specCalc(light, normal_, pos, camPos, specPow, specVal);
	vec4 color = vec4(texture(spec, newUV));
	//vec4 color = vec4(vec3(1),1);
    FragColor = vec4(  ambiant,1) * color;
	
}

vec2 parralax(vec3 camTan, vec3 posTan)
{
	float heightScale = 0.05;
	vec3 viewDir = normalize( posTan - camTan );
	float height =  texture(spec, UV).r;    
    vec2 p = viewDir.xy / viewDir.z * (height * heightScale);
   // return - p;

	//lets try to make the real one.
	float stepSize = 0.1;
	float currentHeight = 1;
	//this vector will go through the layers.
	vec2 stepVector = stepSize * heightScale * viewDir.xy;
	vec2 v = stepVector;
	while( currentHeight > height ) 
	{
		v = v + stepVector;
		currentHeight = currentHeight - stepSize;
		height = texture(spec, UV + v).r;
	}
	return v.xy;
}