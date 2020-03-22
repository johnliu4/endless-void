#include "player.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

using glm::vec3;

Player::Player() : bounding_box(vec3(0.0f, 0.9f, 0.0f), 0.5f, 1.8f, 0.5f) {
	input_manager = InputManager::get_instance();
	prev_cursor_x_pos = input_manager->get_cursor_x_pos();
	prev_cursor_y_pos = input_manager->get_cursor_y_pos();
	yaw_dir = 0.0f;
	pitch_dir = 0.0f;
	mouse_sens_mult = 0.001f;
	max_running_speed = 6.5f;
}

Player::~Player() {}

void Player::poll_input() {
	// get mouse position
	GLfloat delta_cursor_x = input_manager->get_cursor_x_pos() * mouse_sens_mult;
	GLfloat delta_cursor_y = input_manager->get_cursor_y_pos() * mouse_sens_mult;
	input_manager->reset_cursor_pos();


	// keep yaw in range [0, 2pi)
	yaw_dir = glm::mod(yaw_dir - delta_cursor_x, glm::two_pi<GLfloat>());
	// clamp pitch to range [pi/2, -pi/2]
	pitch_dir -= delta_cursor_y;
	if (pitch_dir > glm::half_pi<GLfloat>()) {
		pitch_dir = glm::half_pi<GLfloat>();
	} else if (pitch_dir < -glm::half_pi<GLfloat>()) {
		pitch_dir = -glm::half_pi<GLfloat>();
	}
}

void Player::update() {
	// get movement keys
	bool move_forward = input_manager->get_key(GLFW_KEY_W) == GLFW_PRESS;
	bool move_left = input_manager->get_key(GLFW_KEY_A) == GLFW_PRESS;
	bool move_backward = input_manager->get_key(GLFW_KEY_S) == GLFW_PRESS;
	bool move_right = input_manager->get_key(GLFW_KEY_D) == GLFW_PRESS;

	// check for backward and forward or left and right key presses
	// if both forward and backward keys or left and right keys are pressed
	// then the pair cancels out and does nothing
	if ((move_forward ^ move_backward) || (move_left ^ move_right)) {
		GLfloat yaw_sin = glm::sin(yaw_dir);
		GLfloat yaw_cos = glm::cos(yaw_dir);
		vec3 disp(0.0f);

		if (move_forward) {
			disp.x += yaw_cos;
			disp.z -= yaw_sin;
		}

		if (move_backward) {
			disp.x -= yaw_cos;
			disp.z += yaw_sin;
		}

		if (move_left) {
			disp.x -= yaw_sin;
			disp.z -= yaw_cos;
		}

		if (move_right) {
			disp.x += yaw_sin;
			disp.z += yaw_cos;
		}
		
		disp = glm::normalize(disp) * 3.0f;

		// TODO fix this...
		bounding_box.vel += disp;
		GLfloat vel_len = glm::length(vec3(bounding_box.vel.x, 0.0f, bounding_box.vel.z));
		if (vel_len > max_running_speed) {
			bounding_box.vel.x *= max_running_speed / vel_len;
			bounding_box.vel.z *= max_running_speed / vel_len;
		}
	}

	if (bounding_box.grounded && input_manager->get_key(GLFW_KEY_SPACE)) {
		bounding_box.vel.y += 5.3f;
	}

	//std::cout << bounding_box.pos.x << std::endl;

	//std::cout << pitch_dir << ", " << yaw_dir << std::endl;
	//std::cout << pos.x << ", " << pos.y << ", " << pos.z << std::endl;

	// camera.set_yaw(yaw);
	// camera.set_pitch(pitch);
	//
	// // get the direction of the player from pitch and yaw
	// vec3 dir(1.0f, 0.0f, 0.0f);
	// dir = glm::rotateZ(dir, pitch);
	// dir = glm::rotateY(dir, yaw);
	// dir = glm::normalize(dir);
	//
	// // handle keyboard input
	// vec3 disp(0.0f);
	// bool axial_movement = false;
	// if (input_manager->get_key(GLFW_KEY_W) == GLFW_PRESS) {
	// 	disp.x += glm::cos(yaw);
	// 	disp.z += glm::sin(-yaw);
	// 	axial_movement = true;
	// }
	//
	// if (input_manager->get_key(GLFW_KEY_S) == GLFW_PRESS) {
	// 	disp.x += glm::cos(yaw + pi);
	// 	disp.z += glm::sin(-(yaw + pi));
	// 	if (axial_movement == true) {
	// 		// if both and W and S keys are down they should cancel out
	// 		axial_movement = false;
	// 	} else {
	// 		axial_movement = true;
	// 	}
	// }
	//
	// bool lateral_movement = false;
	// if (input_manager->get_key(GLFW_KEY_A) == GLFW_PRESS) {
	// 	disp.x += glm::cos(yaw + pi * 0.5f);
	// 	disp.z += glm::sin(-(yaw + pi * 0.5f));
	// 	lateral_movement = true;
	// }
	//
	// if (input_manager->get_key(GLFW_KEY_D) == GLFW_PRESS) {
	// 	disp.x += glm::cos(yaw - pi * 0.5f);
	// 	disp.z += glm::sin(-(yaw - pi * 0.5f));
	// 	if (lateral_movement) {
	// 		// if both and A and D keys are down they should cancel out
	// 		lateral_movement = false;
	// 	} else {
	// 		lateral_movement = true;
	// 	}
	// }
	//
	// bool jumping = false;
	// if (bounding_box.grounded) {
	// 	if (input_manager->get_key(GLFW_KEY_SPACE) == GLFW_PRESS) {
	// 		jumping = true;
	// 	}
	// }
	//
	// // determine if player is trying to move
	// if (axial_movement || lateral_movement) {
	// 	vec3 vel = glm::normalize(disp);
	// 	// check if the player is grounded (not in air)
	// 	if (bounding_box.grounded) {
	// 		vel *= run_accel;
	// 		GLfloat current_vel_length = glm::length(bounding_box.vel);
	// 		GLfloat after_vel_length = glm::length(bounding_box.vel + vel);
	//
	// 		// if the player doesn't exceed the max running speed in this frame
	// 		// then simply add the change in velocity to the player
	// 		if (after_vel_length <= max_run_speed) {
	// 			bounding_box.vel += vel;
	// 		} else {
	// 			// if the player will exceed the max running speed in this very frame,
	// 			// cap their speed to the run speed
	// 			if (current_vel_length <= max_run_speed) {
	// 				bounding_box.vel += vel;
	// 				bounding_box.vel = glm::normalize(bounding_box.vel) * max_run_speed;
	// 			} else {
	// 				// if the player already exceeds the max running speed, then do not
	// 				// increase/decrease speed, and only change direction
	// 				// friction from the surface will naturally slow the player down
	// 				bounding_box.vel += vel;
	// 				bounding_box.vel = glm::normalize(bounding_box.vel) * current_vel_length;
	// 			}
	// 		}
	// 	} else {
	// 		// freely add to player velocity in air
	// 		// there is nothing (except air) to slow down player
	// 		vel *= in_air_accel;
	// 		bounding_box.vel.x += vel.x;
	// 		bounding_box.vel.z += vel.z;
	// 	}
	// }
	//
	// if (jumping) {
	// 	bounding_box.vel.y += jump_streng th;
	// }
	//
	// scene->set_camera_pos(bounding_box.pos + glm::vec3(0.0f, 0.75f, 0.0f));
	// scene->set_camera_dir(dir);
	// wireframe->set_pos(bounding_box.pos);
}

void Player::set_pos(vec3 pos) {
	bounding_box.pos = pos;
}

vec3 Player::get_pos() {
	return bounding_box.pos;
}

vec3 Player::get_eye_pos() {
	return bounding_box.pos + vec3(0.0f, 1.8f - 0.1f - 1.8f / 2.0f, 0.0f);
}

GLfloat Player::get_yaw_dir() {
	return yaw_dir;
}

GLfloat Player::get_pitch_dir() {
	return pitch_dir;
}

DynamicAABB& Player::get_bounding_box() {
	return bounding_box;
}
