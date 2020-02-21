#ifndef MATH_H
#define MATH_H

#include "aabb.h"
#include <glm/glm.hpp>

using glm::vec3;

struct intersect_details {
	// whether or not anything
	bool intersects;
	// the scalar that which applied to the displacement
	// vector of a shape gives the moment of intersection
	// with another shape i.e. time of impact on the range [0, 1]
	GLfloat toi;
	// normal of the face on the first shape that intersects with the second shape
	vec3 face_normal;
};

// acceptable error for physics calculations including translations, rotations, etc.
// unitless; but in a computation involving distance for example, 0.001 is 1 millimeters
const GLfloat EPSILON = 0.0001f;
// precomputed value for epsilon squared; may save a sqrt call later
const GLfloat EPSILON_SQR = EPSILON * EPSILON;

/*
 * Math and physics for World Engine.
 * A set of basic collision detection and resolution
 * algorithms are here for use in handling physics.
 */

bool intersects(const AABB& aabb1, const AABB& aabb2);

/* Computes the exact moment of intersection of two given shapes and a given displacement vector.
 * The displacement vector is applied to the first shape given. A scalar in the range [0.0, 1.0]
 * is returned to describe the scalar multiple appplied to the displacement vector that gives
 * an intersection between the two shapes. If no intersection is possible, 1.0 is returned to
 * indicate that applying the displacement ector to the first shape would not collide at all
 * with the second shape.
 */
intersect_details compute_intersect(const AABB& aabb1, const AABB& aabb2, const glm::vec3 disp);

/* Returns the minimum bounding AABB that encloses both given AABBs. */
AABB get_minimum_aabb(const AABB& aabb1, const AABB& aabb2);

/* Returns the squared length of a vector. */
GLfloat length_sqr(vec3& v);

#endif
