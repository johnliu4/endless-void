#ifndef PLATFORM_H
#define PLATFORM_H

#include "entity.h"
#include "src/render/render_entity.h"
#include "src/physics/aabb.h"
#include <glm/glm.hpp>

class Platform : public Entity {
protected:
    // grid position
    glm::ivec3 pos;
    AABB* bounding_box;
    RenderEntity* render_entity;

public:
    static constexpr GLfloat GRID_CELL_WIDTH = 16.0f;

    Platform(glm::ivec3 pos);
    virtual ~Platform();
    virtual void update();

    glm::ivec3 get_pos();
};

#endif
