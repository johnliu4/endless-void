#ifndef PLAYER_H
#define PLAYER_H

#include "physics/dynamic_aabb.h"
#include "src/input/input_manager.h"
#include <glm/glm.hpp>

using glm::vec3;

class Player {
private:

	InputManager* input_manager;
	// direction about player's relative y axis in radians
	// 0 radians is pointing in the (1,0,0) direction
	// value increases turning left
	// range [0, 2pi)
	GLfloat yaw_dir;
	// direction about player's relative x axis in radians
	// 0 radians is pointing in the (0,0,-1) direction
	// value increases going up towards the sky
	// range [pi/2, -pi/2)
	GLfloat pitch_dir;

	double prev_cursor_x_pos;
	double prev_cursor_y_pos;
	// sensitivity multiplier for mouse input
	GLfloat mouse_sens_mult;

	DynamicAABB bounding_box;

	GLfloat max_running_speed;

public:
	Player();
	~Player();

	void poll_input();
	void update();

	void set_pos(vec3 pos);
	vec3 get_pos();
	vec3 get_eye_pos();
	GLfloat get_yaw_dir();
	GLfloat get_pitch_dir();
	DynamicAABB& get_bounding_box();
};

#endif
