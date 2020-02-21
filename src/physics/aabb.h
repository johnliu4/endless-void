#ifndef AABB_H
#define AABB_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>

class AABB {
public:
	AABB(glm::vec3 pos, GLfloat width, GLfloat height, GLfloat length);
	AABB(GLfloat width, GLfloat height, GLfloat length);
	AABB(const AABB& aabb);
	virtual AABB& operator=(const AABB& aabb);

	// position of the box center
	glm::vec3 pos;
	glm::vec3 vel;
	GLfloat width;
	GLfloat height;
	GLfloat length;
	bool grounded;

	/* Returns a string representation of this AABB. */
	std::string to_string() const;

	/* Gets the most negative vertex of the AABB. */
	glm::vec3 get_min() const;
	/* Gets the most negative vertex of the AABB. */
 	glm::vec3 get_max() const;
};

#endif
