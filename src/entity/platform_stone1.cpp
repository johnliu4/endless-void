#include "platform_stone1.h"
#include "src/render/render_engine.h"
#include "src/physics/physics_engine.h"

PlatformStone1::PlatformStone1(glm::ivec3 pos) : Platform(pos) {
    render_entity = RenderEngine::get_instance()->load_wv_obj("resources/platform_stone1.obj");
    render_entity->set_pos((glm::vec3) pos * GRID_CELL_WIDTH + glm::vec3(8.0f, 0.0f, 8.0f));
    bounding_box = new AABB((glm::vec3) pos * GRID_CELL_WIDTH + glm::vec3(8.0f, -0.05f, 8.0f), 16.0f, 0.1f, 16.0f);
    PhysicsEngine::get_instance()->add_static_aabb(bounding_box);
}

PlatformStone1::~PlatformStone1() {
    PhysicsEngine::get_instance()->remove_static_aabb(bounding_box);
    RenderEngine::get_instance()->remove_wv_obj(dynamic_cast<WavefrontOBJEntity*>(render_entity));
}
