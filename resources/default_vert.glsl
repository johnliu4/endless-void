#version 330 core

layout(location = 0) in vec3 vert_pos;
layout(location = 1) in vec2 vert_uv;

out vec2 uv;

uniform mat4 proj_mat;
uniform mat4 view_mat;
uniform mat4 model_mat;

void main() {
	gl_Position = proj_mat * view_mat * model_mat * vec4(vert_pos, 1.0);
	uv = vert_uv;
}
