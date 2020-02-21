#include "render_entity.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

using glm::vec3;
using glm::mat4;

unsigned long long int RenderEntity::render_entity_id_counter = 1;

RenderEntity::RenderEntity() : RENDER_ENTITY_ID(render_entity_id_counter) {
    render_entity_id_counter++;
    pos = vec3(0.0f);
}

void RenderEntity::load_model_matrix(Camera& camera) {
    mat4 model_mat(1.0f);
    model_mat = glm::translate(pos);
    glUniformMatrix4fv(camera.get_model_matrix_id(), 1, GL_FALSE, &model_mat[0][0]);
}

vec3 RenderEntity::get_pos() {
    return pos;
}

void RenderEntity::set_pos(vec3 pos) {
    this->pos = pos;
}

void RenderEntity::set_rot_axis(vec3 axis) {
    rot_axis = axis;
}

void RenderEntity::set_scale(vec3 scale) {
    this->scale = scale;
}

void RenderEntity::set_rot_angle(GLfloat angle) {
    rot_angle = angle;
}

unsigned long long int RenderEntity::get_id() {
    return RENDER_ENTITY_ID;
}
