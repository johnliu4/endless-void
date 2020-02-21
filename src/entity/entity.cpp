#include "entity.h"

unsigned long long int Entity::ID_COUNTER = 1;

Entity::Entity() : id(ID_COUNTER) {
	ID_COUNTER++;
}

unsigned long long int Entity::get_id() {
	return id;
}
