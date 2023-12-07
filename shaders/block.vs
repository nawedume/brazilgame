#version 330 core

layout (location = 0) in vec2 inPos;
layout (location = 1) in vec3 inColor;

out vec3 vColor;

uniform mat3 uViewTransform;

void main() {
	vec2 Pos = (uViewTransform * vec3(inPos, 1.0)).xy;
	gl_Position = vec4(Pos, 0.0, 1.0);
	vColor = inColor; 
}
