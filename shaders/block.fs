#version 330 core

in vec3 vColor;
in vec2 vTexCoord;

out vec4 fColor;

uniform sampler2D spriteTexture;

void main() {
	fColor = texture(spriteTexture, vTexCoord);
}
