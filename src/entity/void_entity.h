/**
 * The Void eats everything.
 * Run away from the Void!
 *
 */

#ifndef VOID_ENTITY_H
#define VOID_ENTITY_H

#include "entity.h"
#include "src/render/render_entity.h"
#include <glm/glm.hpp>

class VoidEntity : public Entity {
private:
    RenderEntity* render_entity;
    glm::vec3 pos;
    glm::vec3 prev_target_pos;
    glm::vec3 target_pos;

public:
    VoidEntity(glm::vec3);
    ~VoidEntity();

    glm::vec3 get_pos();
    void update();
};

#endif
