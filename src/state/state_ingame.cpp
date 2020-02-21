#include "src/physics/physics_engine.h"
#include "src/render/render_engine.h"

#include "state_ingame.h"

StateInGame::StateInGame() {
	world = new World();
}

StateInGame::~StateInGame() {
	delete world;
}

void StateInGame::poll_input() {
	world->poll_input();
}

void StateInGame::update() {
	PhysicsEngine::get_instance()->update();
	world->update();
}