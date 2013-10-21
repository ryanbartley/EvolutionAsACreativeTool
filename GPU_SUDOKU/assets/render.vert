#version 150 core
#extension all : warn

layout (location = 0) in ivec4 positionAndPuzzle;
layout (location = 1) in vec4  screenPosition;
layout (location = 2) in ivec4 matingInfo;

out float value;

void main(void)
{
//	gl_Position = vec4(positionAndPuzzle * 0.03, 1.0);
//	value = matingInfo.x;
	gl_Position = screenPosition;
	value = matingInfo.w;
}
