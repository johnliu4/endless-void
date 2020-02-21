#include "dynamic_aabb.h"

DynamicAABB::DynamicAABB(glm::vec3 pos, GLfloat width, GLfloat height, GLfloat length) : AABB(pos, width, height, length) {
    mass = 80.0f;
    base_friction = 1.0f;
    grounded = false;
}

DynamicAABB::DynamicAABB(GLfloat width, GLfloat height, GLfloat length) : AABB(width, height, length) {
    mass = 80.0f;
    base_friction = 1.0f;
    grounded = false;
}
