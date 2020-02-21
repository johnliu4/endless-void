#include "endless_void.h"
#include <iostream>
#include <time.h>

EndlessVoid::EndlessVoid(int window_width, int window_height) {
	if (!glfwInit()) {
        std::cout << "GLFW failed to initialize." << std::endl;
		return;
	}

    // load OpenGL 3.3 and init glfw
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(window_width, window_height, "Endless Void", nullptr, nullptr);
	if (!window) {
		std::cout << "Failed to create GLFW window." << std::endl;
		glfwTerminate();
		return;
	}

    glfwMakeContextCurrent(window);

    // load glad
	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		std::cout << "GLEW failed to init" << std::endl;
		glfwTerminate();
		return;
	}

	if (glfwRawMouseMotionSupported()) {
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	}

	RenderEngine::init();
	PhysicsEngine::init();
	InputManager::init(window);
}

EndlessVoid::~EndlessVoid() {
	delete current_state;
	InputManager::destroy();
	PhysicsEngine::destroy();
	RenderEngine::destroy();
	glfwTerminate();
}

void EndlessVoid::start() {
	current_state = new StateInGame();
	game_loop();
}

void EndlessVoid::game_loop() {
	bool running = true;

	double before_time = glfwGetTime();
	double fps_timer = 0;
	int frame_count = 0;

	double update_timer = 0;

	InputManager::get_instance()->disable_cursor();

	while (running) {
		double current_time = glfwGetTime();
		double delta_time = current_time - before_time;
		before_time = current_time;

		frame_count++;
		fps_timer += delta_time;
		if (fps_timer >= 1.0) {
			fps_timer -= 1.0;
			glfwSetWindowTitle(window, ("Endless Void FPS: " + std::to_string(frame_count)).c_str());
			frame_count = 0;
		}

		glfwPollEvents();
		current_state->poll_input();

		// ensure updates are done according to physics engine tick rate
		update_timer += delta_time;
		while (update_timer >= PhysicsEngine::TICK_TIME) {
			current_state->update();
			update_timer -= PhysicsEngine::TICK_TIME;
		}

		RenderEngine::get_instance()->render(delta_time, update_timer / PhysicsEngine::TICK_TIME);

		glfwSwapBuffers(window);
		if (glfwWindowShouldClose(window)) {
			running = false;
		}
	}
}
