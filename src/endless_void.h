#ifndef ENDLESS_VOID_H
#define ENDLESS_VOID_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "input/input_manager.h"
#include "physics/physics_engine.h"
#include "render/render_engine.h"
#include "state/state.h"
#include "state/state_ingame.h"

class EndlessVoid {
private:
	GLFWwindow* window;
	GameState* current_state;
	void game_loop();
public:
	// updates per second
	static const int TICK_RATE = 30;
	static constexpr GLfloat TICK_TIME = 1.0f / (GLfloat) TICK_RATE;

	EndlessVoid(unsigned int window_width, unsigned int window_height);
	~EndlessVoid();
	void start();
};

#endif
