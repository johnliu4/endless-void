#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class InputManager {
private:
	static InputManager* singleton;
    GLFWwindow* glfw_window;

	InputManager(GLFWwindow* window);
    ~InputManager();

public:
    // singleton methods
	static InputManager* get_instance();
    // must be called before any call to get_instance()
    static void init(GLFWwindow* window);
    static void destroy();

	// get the state of a key
	int get_key(int glfw_key);

	// sets the cursor position to (0, 0)
	void reset_cursor_pos();

	// enables the cursor if it is disabled
	void enable_cursor();

	// hides the cursor and locks it to the window
	// GLFW takes care of cursor re-centering and deltas
	void disable_cursor();

	double get_cursor_x_pos();
	double get_cursor_y_pos();
};

#endif
