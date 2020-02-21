#ifndef WORLD_H
#define WORLD_H

#include "player.h"
#include "path_generator.h"
#include "render/camera.h"

class World {
private:
	PathGenerator path_gen;
	Player player;
	Camera* camera;

public:
	World();
	~World();

	void poll_input();
	void update();
};

#endif
