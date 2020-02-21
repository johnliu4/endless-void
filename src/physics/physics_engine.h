#ifndef PHYSICS_ENGINE_H
#define PHYSICS_ENGINE_H

#include "aabb.h"
#include "dynamic_aabb.h"
#include <glad/glad.h>
#include <list>

class PhysicsEngine {
private:
	static PhysicsEngine* singleton;
    PhysicsEngine();
    ~PhysicsEngine();

	GLfloat gravity;

	DynamicAABB* dynamic_aabb;
	std::list<AABB*> static_aabbs;

public:
    // singleton methods
    static PhysicsEngine* get_instance();
    // must be called before any call to get_instance()
    static void init();
    static void destroy();

	// updates per second
	static const int TICK_RATE = 30;
	// time between updates
	static constexpr GLfloat TICK_TIME = 1.0f / (GLfloat) (TICK_RATE);

	void update();

	void set_dynamic_aabb(DynamicAABB* aabb);
	void add_static_aabb(AABB* aabb);
	void remove_static_aabb(AABB* aabb);
};

#endif
