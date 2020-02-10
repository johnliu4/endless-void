#version 330 core

layout(location = 0) in vec3 vert_pos_modelspace;

uniform mat4 mvp;

void main() {
	gl_Position = mvp * vec4(vert_pos_modelspace, 1.0);
}