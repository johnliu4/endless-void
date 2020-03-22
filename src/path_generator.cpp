#include "path_generator.h"
#include "entity/platform_stone1.h"
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <stack>
#include <cmath>


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

glm::vec3 PathNode::get_pos() {
	return platform->get_pos();
}

PathGenerator::PathGenerator() {
	root_node = nullptr;
	player_node = nullptr;
	int start_x = -4.0f;
	int start_z = 0.0f;

	root_node = new PathNode(new PlatformStone1(glm::ivec3(start_x, 0, start_z)));
	PathNode* node = root_node;

	for (int i = 0; i < 5; i++) {
		node->forward = new PathNode(
			new PlatformStone1(glm::ivec3(start_x + i, 0, start_z)));
		node = node->forward;
	}

	void_entity = new VoidEntity(vec3((start_x + 0.5f) * Platform::GRID_CELL_WIDTH, 0.0f, (start_z + 0.5f) * Platform::GRID_CELL_WIDTH));
	player_node = root_node;
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

void PathGenerator::find_player_node(Player& player) {
	std::cout << "recomputed!" << std::endl;

	// iterate through all nodes and find closest path node
	std::stack<PathNode*> nodes;
	nodes.push(root_node);
	GLfloat min_dist = INFINITY;
	PathNode* closest = nullptr;
	while (nodes.size() != 0) {
		PathNode* current = nodes.top();
		nodes.pop();
		if (current == nullptr) {
			continue;
		} else {
			GLfloat len = glm::length(player.get_pos() -
				(current->get_pos() * Platform::GRID_CELL_WIDTH + glm::vec3(Platform::GRID_CELL_WIDTH / 2.0f, 0.0f, Platform::GRID_CELL_WIDTH / 2.0f)));
			if (len < min_dist) {
				min_dist = len;
				closest = current;
			}

			nodes.push(current->left);
			nodes.push(current->forward);
			nodes.push(current->right);
		}
	}

	player_node = closest;
}

void PathGenerator::update(Player& player) {
	vec3 player_pos = player.get_pos();
	glm::ivec3 player_grid_pos = glm::ivec3(
		glm::floor(player_pos.x / Platform::GRID_CELL_WIDTH),
		glm::floor(player_pos.y / Platform::GRID_CELL_WIDTH),
		glm::floor(player_pos.z / Platform::GRID_CELL_WIDTH));
	vec3 void_pos = void_entity->get_pos();
              
	// if the player is not at their previous integer grid position anymore
	// then re-find the node closest to them
	// and generate new pathing if needed
	if (player_grid_pos.x != player_node->get_pos().x ||
		player_grid_pos.z != player_node->get_pos().z) {
		find_player_node(player);

		// 
	}

	//std::cout << "p:" << glm::to_string(player_pos) << std::endl;
	//std::cout << "v:" << glm::to_string(void_pos) << std::endl;
}
