#ifndef PLATFORM_STONE1_H
#define PLATFORM_STONE1_H

#include "platform.h"
#include "src/physics/aabb.h"
#include <glm/glm.hpp>

class PlatformStone1 : public Platform {
public:
    PlatformStone1(glm::ivec3 pos);
    ~PlatformStone1();
};

#endif
