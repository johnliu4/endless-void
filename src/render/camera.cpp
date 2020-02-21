#include "camera.h"
#include "render_loader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <iostream>

using glm::vec3;
using glm::mat4;

Camera::Camera() {
    GLuint vertex_shader_id = load_shader("resources/default_vert.glsl", GL_VERTEX_SHADER);
    GLuint fragment_shader_id = load_shader("resources/default_frag.glsl", GL_FRAGMENT_SHADER);
    program_id = glCreateProgram();
    glAttachShader(program_id, vertex_shader_id);
    glAttachShader(program_id, fragment_shader_id);
    glLinkProgram(program_id);
    glDetachShader(program_id, vertex_shader_id);
    glDetachShader(program_id, fragment_shader_id);
    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    proj_mat_id = glGetUniformLocation(program_id, "proj_mat");
    view_mat_id = glGetUniformLocation(program_id, "view_mat");
    model_mat_id = glGetUniformLocation(program_id, "model_mat");

    proj_mat = glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 100.0f);
    view_mat = glm::lookAt(
        vec3(0.0f, 0.0f, 0.0f),
        vec3(1.0f, 0.0f, 0.0f),
        vec3(0.0f, 1.0f, 0.0f));
}

void Camera::use_program(GLfloat update_lag) {
    glUseProgram(program_id);
    // create a direction vector from the pitch and yaw
    vec3 dir(1.0f, 0.0f, 0.0f);
    dir = glm::rotateZ(dir, pitch_dir);
    dir = glm::rotateY(dir, yaw_dir);

    vec3 linear_interp = (pos - prev_pos) * update_lag;
    
    view_mat = glm::lookAt(
        prev_pos + linear_interp,
        prev_pos + linear_interp + dir,
        vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(proj_mat_id, 1, GL_FALSE, &proj_mat[0][0]);
    glUniformMatrix4fv(view_mat_id, 1, GL_FALSE, &view_mat[0][0]);
}

GLuint Camera::get_model_matrix_id() {
    return model_mat_id;
}

void Camera::set_pos(vec3 pos) {
    prev_pos = this->pos;
    this->pos = pos;
}

void Camera::set_yaw_dir(GLfloat yaw) {
    yaw_dir = yaw;
}

void Camera::set_pitch_dir(GLfloat pitch) {
    pitch_dir = pitch;
}
