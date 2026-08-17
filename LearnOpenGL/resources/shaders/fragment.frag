#version 330 core

in vec2 texPos;

out vec4 outputDisplay;

uniform sampler2D textureID;

void main() {
	outputDisplay = texture(textureID, texPos);
}