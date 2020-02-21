#include "math.h"
#include <glm/gtx/string_cast.hpp>
#include <limits>
#include <iostream>

using glm::vec3;

/* Checks distance between centers of box on each axis. */
bool intersects(const AABB& aabb1, const AABB& aabb2) {
	if (glm::abs(aabb2.pos.x - aabb1.pos.x)
		< aabb1.width / 2.0f + aabb2.width / 2.0f - EPSILON) {
		if (glm::abs(aabb2.pos.y - aabb1.pos.y)
			< aabb1.height / 2.0f + aabb2.height / 2.0f - EPSILON) {
			if (glm::abs(aabb2.pos.z - aabb1.pos.z)
				< aabb1.length / 2.0f + aabb2.length / 2.0f - EPSILON) {


				//std::cout << "z diff: " << (glm::abs(aabb2.pos.z - aabb1.pos.z)) << std::endl;
				//std::cout << "length diff: " << (aabb1.length / 2.0f + aabb2.length / 2.0f - EPSILON) << std::endl;

				return true;
			}
		}
	}

	return false;
}

intersect_details compute_intersect(const AABB& aabb1, const AABB& aabb2, const glm::vec3 disp) {
	// the minkowski difference of two AABBs always yields another AABB
	// therefore we only need to find the min and max vertices
	vec3 mink_min = aabb2.get_min() - aabb1.get_max();
	vec3 mink_max = aabb2.get_max() - aabb1.get_min();

	// std::cout << "min: " << glm::to_string(mink_min) << std::endl;
	// std::cout << "max: " << glm::to_string(mink_max) << std::endl;

	// the minkowski AABB represents a space where if you extend a ray from the origin
	// and it collides with the minkowski AABB, then the same ray applied to the displacement
	// of the first shape will collide with the second shape
	// so first check if the minkowski difference contains the origin
	// if it does, then the AABBs are already intersecting

	// check if the displacement vector collides with each of the 6 minkowski AABB faces from the origin
	// and find the closest intersection if it exists
	bool intersects = false;
	GLfloat min_intersection = 1.0f;
	glm::vec3 intersect_normal(0.0f);

	// left face
	if (disp.x > 0.0f && mink_min.x >= -EPSILON) {
		GLfloat scalar_intersection = mink_min.x / disp.x;
		glm::vec3 intersection_point = disp * scalar_intersection;
		// check if intersection point is inside the rectangular face on yz plane
		if (intersection_point.y >= mink_min.y && intersection_point.y <= mink_max.y
			&& intersection_point.z >= mink_min.z && intersection_point.z <= mink_max.z) {
			if (scalar_intersection < min_intersection) {
				min_intersection = scalar_intersection;
				intersect_normal = glm::vec3(1.0f, 0.0f, 0.0f);
				intersects = true;
			}
		}
	}

	// right face
	if (disp.x < 0.0f && mink_max.x <= EPSILON) {
		GLfloat scalar_intersection = glm::abs(mink_max.x / disp.x);
		glm::vec3 intersection_point = disp * scalar_intersection;
		// check if intersection point is inside the rectangular face on yz plane
		if (intersection_point.y >= mink_min.y && intersection_point.y <= mink_max.y
			&& intersection_point.z >= mink_min.z && intersection_point.z <= mink_max.z) {
			if (scalar_intersection < min_intersection) {
				min_intersection = scalar_intersection;
				intersect_normal = glm::vec3(-1.0f, 0.0f, 0.0f);
				intersects = true;
			}
		}
	}

	// bottom face
	if (disp.y > 0.0f && mink_min.y >= -EPSILON) {
		GLfloat scalar_intersection = glm::abs(mink_min.y / disp.y);
		glm::vec3 intersection_point = disp * scalar_intersection;
		// check if intersection point is inside the rectangular face on xz plane
		if (intersection_point.x >= mink_min.x && intersection_point.x <= mink_max.x
			&& intersection_point.z >= mink_min.z && intersection_point.z <= mink_max.z) {
			if (scalar_intersection < min_intersection) {
				min_intersection = scalar_intersection;
				intersect_normal = glm::vec3(0.0f, -1.0f, 0.0f);
				intersects = true;
			}
		}
	}

	// top face
	if (disp.y < 0.0f && mink_max.y <= EPSILON) {
		GLfloat scalar_intersection = mink_max.y / disp.y;
		glm::vec3 intersection_point = disp * scalar_intersection;
		// check if intersection point is inside the rectangular face on xz plane
		if (intersection_point.x >= mink_min.x && intersection_point.x <= mink_max.x
			&& intersection_point.z >= mink_min.z && intersection_point.z <= mink_max.z) {
			if (scalar_intersection < min_intersection) {
				min_intersection = scalar_intersection;
				intersect_normal = glm::vec3(0.0f, 1.0f, 0.0f);
				intersects = true;
			}
		}
	}

	// back face
	if (disp.z > 0.0f && mink_min.z >= -EPSILON) {
		GLfloat scalar_intersection = mink_min.z / disp.z;
		glm::vec3 intersection_point = disp * scalar_intersection;
		// check if intersection point is inside the rectangular face on xy plane
		if (intersection_point.x >= mink_min.x && intersection_point.x <= mink_max.x
			&& intersection_point.y >= mink_min.y && intersection_point.y <= mink_max.y) {
			if (scalar_intersection < min_intersection) {
				min_intersection = scalar_intersection;
				intersect_normal = glm::vec3(0.0f, 0.0f, 1.0f);
				intersects = true;
			}
		}
	}

	// front face
	if (disp.z < 0.0f && mink_max.z <= EPSILON) {
		GLfloat scalar_intersection = glm::abs(mink_max.z / disp.z);
		glm::vec3 intersection_point = disp * scalar_intersection;
		// check if intersection point is inside the rectangular face on xy plane
		if (intersection_point.x >= mink_min.x && intersection_point.x <= mink_max.x
			&& intersection_point.y >= mink_min.y && intersection_point.y <= mink_max.y) {
			if (scalar_intersection < min_intersection) {
				min_intersection = scalar_intersection;
				intersect_normal = glm::vec3(0.0f, 0.0f, -1.0f);
				intersects = true;
			}
		}
	}

	return { intersects, min_intersection, intersect_normal };
}

AABB get_minimum_aabb(const AABB& aabb1, const AABB& aabb2) {
	glm::vec3 half1(aabb1.width / 2.0f, aabb1.height / 2.0f, aabb1.length / 2.0f);
	glm::vec3 half2(aabb2.width / 2.0f, aabb2.height / 2.0f, aabb2.length / 2.0f);
	glm::vec3 min(
		glm::min(aabb1.pos.x - half1.x, aabb2.pos.x - half2.x),
		glm::min(aabb1.pos.y - half1.y, aabb2.pos.y - half2.y),
		glm::min(aabb1.pos.z - half1.z, aabb2.pos.z - half2.z));
	glm::vec3 max(
		glm::max(aabb1.pos.x + half1.x, aabb2.pos.x + half2.x),
		glm::max(aabb1.pos.y + half1.y, aabb2.pos.y + half2.y),
		glm::max(aabb1.pos.z + half1.z, aabb2.pos.z + half2.z));
	return AABB(
		glm::vec3((max.x + min.x) / 2.0f, (max.y + min.y) / 2.0f, (max.z + min.z) / 2.0f),
		max.x - min.x, max.y - min.y, max.z - min.z);
}

GLfloat length_sqr(vec3& v) {
	return v.x * v.x + v.y * v.y + v.z * v.z;
}