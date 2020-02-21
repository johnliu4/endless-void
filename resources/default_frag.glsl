#version 330 core

in vec2 uv;
out vec3 color;

uniform sampler2D text_sampler;

void main() {
	color = texture(text_sampler, uv).rgb;
}
