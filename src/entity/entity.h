#ifndef ENTITY_H
#define ENTITY_H

/* An interactable game entity. */

class Entity {
private:
	static unsigned long long int ID_COUNTER;
	// a unique id to represent an instance of an entity
	const unsigned long long int id;

public:
	Entity();
	virtual void update() = 0;	
	unsigned long long int get_id();
};

#endif
