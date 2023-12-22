#version 330 core

layout (location = 0) in vec2 inPos;
layout (location = 2) in vec2 inTexCoord;

out vec2 vTexCoord;

uniform float uZIndex; 

void main() {
	gl_Position = vec4(inPos, uZIndex, 1.0);
	vTexCoord = inTexCoord;
}
