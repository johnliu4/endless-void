#ifndef WORLD_H
#define WORLD_H

#include "player.h"
#include "path_generator.h"
#include "render/camera.h"
#include "render/render_gui.h"

class World {
private:
	PathGenerator path_gen;
	Player player;
	RenderGui gui;
	Camera* camera;

public:
	World();
	~World();

	void poll_input();
	void update();
};

#endif
