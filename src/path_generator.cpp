#include "path_generator.h"
#include "entity/platform_stone1.h"
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <stack>

using glm::vec3;

PathNode::PathNode(Platform* platform) {
	this->platform = platform;
	parent = nullptr;
	left = nullptr;
	forward = nullptr;
	right = nullptr;
}

PathNode::~PathNode() {
	delete platform;
}

PathGenerator::PathGenerator() {
	root_node = nullptr;
	int start_x = -4.0f;
	int start_z = 0.0f;

	root_node = new PathNode(new PlatformStone1(vec3(start_x * GRID_CELL_WIDTH, 0.0f, start_z * GRID_CELL_WIDTH)));
	PathNode* node = root_node;

	for (int i = 0; i < 5; i++) {
		node->forward = new PathNode(
			new PlatformStone1(vec3((start_x + i) * GRID_CELL_WIDTH, 0.0f, start_z * GRID_CELL_WIDTH)));
		node = node->forward;
	}

	void_entity = new VoidEntity(vec3((start_x + 0.5f) * GRID_CELL_WIDTH, 0.0f, (start_z + 0.5f) * GRID_CELL_WIDTH));
}

PathGenerator::~PathGenerator() {
	// delete all nodes starting from the parent
	std::stack<PathNode*> node_stack;
	node_stack.push(root_node);
	while (node_stack.size() != 0) {
		PathNode* p = node_stack.top();
		if (p == nullptr) {
			node_stack.pop();
			continue;
		} else {
			node_stack.pop();
			node_stack.push(p->left);
			node_stack.push(p->forward);
			node_stack.push(p->right);
			delete p;
		}
	}
}

void PathGenerator::update(Player& player) {
	vec3 player_pos = player.get_pos();
	vec3 void_pos = void_entity->get_pos();

	//std::cout << "p:" << glm::to_string(player_pos) << std::endl;
	//std::cout << "v:" << glm::to_string(void_pos) << std::endl;
}
