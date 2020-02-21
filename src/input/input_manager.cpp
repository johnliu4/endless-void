#include <iostream>

#include "input_manager.h"

InputManager* InputManager::singleton = nullptr;

InputManager::InputManager(GLFWwindow* window) {
    glfw_window = window;
}

InputManager::~InputManager() {}

InputManager* InputManager::get_instance() {
	return InputManager::singleton;
}

void InputManager::init(GLFWwindow* window) {
    InputManager::singleton = new InputManager(window);
}

void InputManager::destroy() {
    delete InputManager::singleton;
}

int InputManager::get_key(int glfw_key) {
    return glfwGetKey(glfw_window, glfw_key);
}

void InputManager::reset_cursor_pos() {
    glfwSetCursorPos(glfw_window, 0, 0);
};

void InputManager::enable_cursor() {
    glfwSetInputMode(glfw_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void InputManager::disable_cursor() {
    glfwSetInputMode(glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

double InputManager::get_cursor_x_pos() {
    double x, y;
    glfwGetCursorPos(glfw_window, &x, &y);
    return x;
}

double InputManager::get_cursor_y_pos() {
    double x, y;
    glfwGetCursorPos(glfw_window, &x, &y);
    return y;
}
