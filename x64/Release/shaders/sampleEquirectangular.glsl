#version 430 core

vec4 sampleEquirectangularMip(sampler2D sampleIn, vec3 dir, float mipMap)
{
	vec3 pos = normalize(vec3(dir.x, dir.y, dir.z));
	float theta = acos(pos.y);
	float phi = atan(pos.x, -pos.z);
	vec4 sampleValue = texture(sampleIn, vec2(
		0.5 + phi / 6.2831852,
		theta / 3.1415926),mipMap);

	return sampleValue;
}

vec4 sampleEquirectangular(sampler2D sampleIn, vec3 dir)
{
	vec3 pos = normalize(vec3(dir.x, dir.y, dir.z));
	float theta = acos(pos.y);
	float phi = atan(pos.x, -pos.z);
	vec4 sampleValue = texture(sampleIn, vec2(
		0.5 + phi / 6.2831852,
		theta / 3.1415926));

	return sampleValue;
}

