#version 150 core
#extension all : warn

layout (location = 0) in float numbers;
layout (location = 2) in vec2  screenSpace;

uniform samplerBuffer sudoku_count;

out float vs_numbers;

void main() {
	vs_numbers = numbers;
	gl_Position = vec4(screenSpace, 0.0, 1.0);
}
