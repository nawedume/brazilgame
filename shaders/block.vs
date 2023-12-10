#version 330 core

layout (location = 0) in vec2 inPos;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec2 inTexCoord;

out vec2 vTexCoord;
out vec3 vColor;

uniform mat3 uViewTransform;
uniform vec2 uPos;
uniform vec2 uScale;
uniform vec2 uTexOffset;

void main() {
	vec2 Pos = ((inPos * uScale) + uPos);
	Pos = (Pos * 2.0) - 1.0;
	gl_Position = vec4(Pos, 0.0, 1.0);
	vColor = inColor; 
	vTexCoord = inTexCoord + uTexOffset;
}
