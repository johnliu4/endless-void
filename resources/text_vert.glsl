#version 330 core
layout (location = 0) in vec4 vert_pos;
out vec2 tex_coords;

uniform mat4 proj_mat;

void main() {
    gl_Position = proj_mat * vec4(vert_pos.xy, 0.0, 1.0);
    tex_coords = vert_pos.zw;
}
