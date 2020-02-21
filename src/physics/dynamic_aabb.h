#ifndef DYNAMIC_AABB_H
#define DYNAMIC_AABB_H

#include "aabb.h"

class DynamicAABB : public AABB {
public:
    DynamicAABB(glm::vec3 pos, GLfloat width, GLfloat height, GLfloat length);
	DynamicAABB(GLfloat width, GLfloat height, GLfloat length);

    GLfloat mass;
    GLfloat base_friction;
    bool grounded;
};

#endif