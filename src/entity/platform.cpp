#include "platform.h"
#include <iostream>

Platform::Platform(glm::ivec3 pos) {
    this->pos = pos;
}

Platform::~Platform() {}

void Platform::update() {}

glm::ivec3 Platform::get_pos() {
    return pos;
}