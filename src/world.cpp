#include "world.h"
#include "render/render_engine.h"
#include "physics/physics_engine.h"
#include <iostream>

World::World() {
    camera = &(RenderEngine::get_instance()->get_camera());
    player.set_pos(glm::vec3(PathGenerator::GRID_CELL_WIDTH * 0.5f, 0.9f, PathGenerator::GRID_CELL_WIDTH * 0.5f));
    PhysicsEngine* physics_engine = PhysicsEngine::get_instance();
    physics_engine->set_dynamic_aabb(&(player.get_bounding_box()));
}

World::~World() {}

void World::poll_input() {
    player.poll_input();
    camera->set_yaw_dir(player.get_yaw_dir());
    camera->set_pitch_dir(player.get_pitch_dir());
}

void World::update() {
    player.update();
    camera->set_pos(player.get_eye_pos());
    path_gen.update(player);
}
