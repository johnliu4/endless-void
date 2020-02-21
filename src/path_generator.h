/**
 * Path generation.
 *
 * The world is divided into a grid with unit length 16 meters.
 *
 * Different types of platforms exist, and they should all
 * measure 16m x 16m. The platforms are generated as the player
 * moves. Old platforms are removed as the player moves farther
 * from them.
 *
 */


#ifndef PATH_GENERATOR_H
#define PATH_GENERATOR_H

#include "player.h"
#include "entity/platform.h"
#include "entity/void_entity.h"
#include <glm/glm.hpp>


/* Generator for a randomized game world. */

class PathGenerator;

class PathNode {
friend class PathGenerator;
private:
	Platform* platform;
	PathNode* parent;
	// nodes relative to this node and its parent
	PathNode* left;
	PathNode* forward;
	PathNode* right;

public:
	PathNode(Platform* platform);
	~PathNode();
	glm::vec3 get_pos();
};

class PathGenerator {
private:
	// pointer to the node farthest back
	// this node is also where the Void is
	PathNode* root_node;
	Player* player;
	VoidEntity* void_entity;

public:
	static constexpr GLfloat GRID_CELL_WIDTH = 16.0f;

	PathGenerator();
	~PathGenerator();

	void update(Player& player);
};

#endif
