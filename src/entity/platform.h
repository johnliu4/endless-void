#ifndef PLATFORM_H
#define PLATFORM_H

#include "entity.h"
#include "src/render/render_entity.h"
#include "src/physics/aabb.h"
#include <glm/glm.hpp>

class Platform : public Entity {
protected:
    AABB* bounding_box;
    RenderEntity* render_entity;

public:
    Platform();
    virtual ~Platform();
    virtual void update();
};

#endif
