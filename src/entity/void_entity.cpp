#include "void_entity.h"
#include "src/render/render_engine.h"

VoidEntity::VoidEntity(glm::vec3 pos) {
    render_entity = RenderEngine::get_instance()->load_wv_obj("resources/void.obj");
    render_entity->set_pos(pos);
    this->pos = pos;
}

VoidEntity::~VoidEntity() {
    RenderEngine::get_instance()->remove_wv_obj(dynamic_cast<WavefrontOBJEntity*>(render_entity));
}

glm::vec3 VoidEntity::get_pos() {
    return render_entity->get_pos();
}

void VoidEntity::update() {
    render_entity->set_pos(pos);
}
