#include "physics_engine.h"
#include "math.h"
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/projection.hpp>
#include <iostream>

using glm::vec3;

PhysicsEngine* PhysicsEngine::singleton = nullptr;

PhysicsEngine::PhysicsEngine() {
    gravity = 9.81f;
	dynamic_aabb = nullptr;
}

PhysicsEngine::~PhysicsEngine() {}

PhysicsEngine* PhysicsEngine::get_instance() {
    return PhysicsEngine::singleton;
}

void PhysicsEngine::init() {
    PhysicsEngine::singleton = new PhysicsEngine();
}

void PhysicsEngine::destroy() {
    delete PhysicsEngine::singleton;
}

/* Performs one update in the physics world. A length of TICK_TIME seconds is elapsed */
void PhysicsEngine::update() {
	DynamicAABB* dyn_aabb = dynamic_aabb;
    if (dyn_aabb != nullptr) {
		// std::cout << glm::to_string(player_aabb->pos) << std::endl;
		// std::cout << glm::to_string(dyn_aabb->vel) << std::endl;

		dyn_aabb->vel.y -= gravity * TICK_TIME;
		vec3 current_disp = dyn_aabb->vel * TICK_TIME;
		DynamicAABB& aabb = *dyn_aabb;
		dyn_aabb->grounded = false;


		while (length_sqr(current_disp) >= EPSILON_SQR) {
			// find the maximum distance that the AABB can move without intersecting anything
			intersect_details min_isect = { false, 1.0f };
			AABB before_aabb = aabb;
			// compute an aabb enclosing the before and after positions of the aabb
			// this larger aabb is used to quickly determine if the dynamic aabb
			// might intersect a static aabb
			AABB after_aabb = before_aabb;
			after_aabb.pos += current_disp;
			AABB min_enclosing = get_minimum_aabb(before_aabb, after_aabb);

			// std::cout << min_enclosing.to_string() << std::endl;

			// compute the physics between the dynamic aabb and every static aabb to find the minimum intersect
			/** TODO space partitioning to reduce the number of static aabs to check */
			for (std::list<AABB*>::iterator it_static = static_aabbs.begin();
				it_static != static_aabbs.end(); it_static++) {
				// compare the min_enclosing aabb with the static aabb
				if (intersects(min_enclosing, **it_static)) {
					intersect_details details = compute_intersect(before_aabb, **it_static, current_disp);
					// if there is an intersection, compare it with the current min_isect
					if (details.intersects) {
						if (!min_isect.intersects) {
							min_isect = details;
						} else if (details.toi < min_isect.toi) {
							min_isect = details;
						}
					}
				}
			}

			if (min_isect.intersects) {
				// move dynamic aabb to point of intersection
				aabb.pos += min_isect.toi * current_disp;
				// reduce current_disp by the amount moved
				current_disp *= (1.0f - min_isect.toi);

				// reduce current_disp by its component along the
				// static aabb intersecting face normal
				// essentially representing an inelastic collision with the wall
				// a dynamic aabb that collides with a wall will then "glue" against the wall
				// and keep moving along the plane of the aabb
				vec3 normal_proj = glm::proj(current_disp, min_isect.face_normal);



				current_disp -= normal_proj;
				aabb.vel = current_disp / TICK_TIME;

				// apply basic friction based on dynamic aabb mass and gravity
				// if the intersecting face normal is pointing towards positive y
				if (min_isect.face_normal.y > 0.0f) {
					// force of friction = normal force * mu
					// normal force = component of gravity along intersecting face normal = m * g_face_normal
					// Newton's Second law, F = ma
					// m * g_face_normal * mu = ma
					// g_face_normal * mu = a
					// then delta velocity = g_face_normal * mu * TICK_TIME
					GLfloat friction_dvel = glm::length(glm::proj(vec3(0.0f, 1.0f, 0.0f) * gravity, min_isect.face_normal)) * dyn_aabb->base_friction;

					if (length_sqr(aabb.vel) > friction_dvel * friction_dvel) {
						aabb.vel -= glm::normalize(aabb.vel) * friction_dvel;
					} else {
						aabb.vel = vec3(0.0f);
					}

					dynamic_aabb->grounded = true;
				}

			} else {
				aabb.pos += current_disp;
				// dynamic aabb did not intersect with anything; must be in the air
				break;
			}
		}

    }
	// if (player_aabb != nullptr) {
	// 	player_aabb->vel.y -= gravity * TICK_TIME;
	// 	GLfloat prev_vel_y = dynamic_aabb->vel.y;
	// 	AABB aabb = *dynamic_aabb;
	// 	glm::vec3 current_disp = aabb.vel * TICK_TIME;
    //
	// 	while (glm::length(current_disp) >= EPSILON) {
	// 		// find the maximum distance that the AABB can move without intersecting with anything
	// 		intersect_details min_intersect = { false, 1.0f };
	// 		AABB before_aabb = aabb;
	// 		aabb.pos += current_disp;
	// 		AABB min_enclosing = get_minimum_aabb(before_aabb, aabb);
    //
	// 		for (auto it_static = static_aabbs.begin(); it_static != static_aabbs.end(); it_static++) {
	// 			if (intersects(min_enclosing, **it_static)) {
	// 				intersect_details details = compute_intersect(before_aabb, **it_static, current_disp);
    //
	// 				if (!min_intersect.intersects) {
	// 					min_intersect = details;
	// 				} else if (details.scalar < min_intersect.scalar) {
	// 					min_intersect.scalar = details.scalar;
	// 					min_intersect.face_normal = details.face_normal;
	// 				}
	// 			}
	// 		}
    //
	// 		if (min_intersect.intersects) {
	// 			// move aabb back to the position of intersection
	// 			aabb.pos -= (1.0f - min_intersect.scalar) * current_disp;
    //
	// 			current_disp *= 1.0f - min_intersect.scalar;
	// 			glm::vec3 normal_proj = glm::proj(current_disp, min_intersect.face_normal);
	// 			current_disp -= normal_proj;
    //
	// 			aabb.vel = current_disp / TICK_TIME;
    //
	// 			// apply friction due to gravity
	// 			glm::vec3 xz_vel(aabb.vel.x, 0.0f, aabb.vel.z);
	// 			if (glm::length(xz_vel) > EPSILON && glm::abs(glm::dot(glm::vec3(0.0f, 1.0f, 0.0f), min_intersect.face_normal)) >= 1.0f - EPSILON) {
	// 				glm::vec3 frictional_slow = glm::normalize(xz_vel) * dynamic_aabb->base_friction * gravity * TICK_TIME;
	// 				if (glm::length(frictional_slow) > glm::length(xz_vel)) {
	// 					aabb.vel.x = 0.0f;
	// 					aabb.vel.z = 0.0f;
	// 				} else {
	// 					aabb.vel -= frictional_slow;
	// 				}
	// 			}
	// 		} else {
	// 			break;
	// 		}
	// 	}
    //
	// 	// if the box was previously moving downwards (or was not moving)
	// 	// and is now not moving in the y-axis then it must be grounded
	// 	aabb.grounded = prev_vel_y <= 0.0f && glm::abs(aabb.vel.y) < EPSILON;
	// 	*dynamic_aabb = aabb;
	// }
}

void PhysicsEngine::set_dynamic_aabb(DynamicAABB* aabb) {
	dynamic_aabb = aabb;
}

void PhysicsEngine::add_static_aabb(AABB* aabb) {
	static_aabbs.push_back(aabb);
}

void PhysicsEngine::remove_static_aabb(AABB* aabb) {
	static_aabbs.remove(aabb);
}
