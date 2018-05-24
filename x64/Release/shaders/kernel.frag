#version 430 core

void makeOffsetMat(float offset, inout vec2[9] offsetMat)
{
	offsetMat[0] = vec2(-offset, offset);						//top left
	offsetMat[1] =	vec2(0, offset);							//top middle
	offsetMat[2] =	vec2(offset, offset);						// top right
	offsetMat[3] =	vec2(-offset, 0);							// middle right
	offsetMat[4] =	vec2(0,0);									//middle
	offsetMat[5] =	vec2(offset, 0);							//middle right
	offsetMat[6] =	vec2(-offset, -offset);						//bottom left
	offsetMat[7] =	vec2(0, -offset);							//bottom middle
	offsetMat[8] =	vec2(offset, -offset);						//bottom left
}