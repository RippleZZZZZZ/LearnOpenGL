#version 330 core

in vec3 outPos;

out vec4 outputColor;

uniform vec3 color;

void main() {
	outputColor = vec4(color, 1.0f);
}