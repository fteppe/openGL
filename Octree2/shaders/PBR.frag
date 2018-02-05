 #version 330 core
in vec3 normal;
in vec3 vertexColor;
in vec3 vertexPos;
in vec2 UV;
in vec3 posTan;
in vec3 camTan;
in vec3 tang;
 
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
	vec3 bumpVal = texture(bump, newUV).rgb;
	normal_ = normalize(normal + bumpVal);
	
	vec4 specularity = vec4(0.2);//vec4(texture(spec, newUV));
	float specVal = specularity.r * 10;
	float specPow = 32;
	//We use the spec map as a bump map as well, to make it look a bit better
	//we add a constant value to the intensity, so it is never dark.
	vec3 ambiant = vec3(0.5);
	vec3 intensityVec = fragLight(light, normal_, pos);
	vec3 specVec = specCalc(light, normal_, pos, camPos, specPow, specVal);
	vec4 color = vec4(texture(diffuse, newUV));
	//vec4 color = vec4(vec3(1),1);
    FragColor = vec4( specVec + intensityVec + ambiant,1) * color;
	
}

vec2 parralax(vec3 camTan, vec3 posTan)
{
	vec3 camDir = posTan - camTan;
	return (normalize(camDir)).xy;
}