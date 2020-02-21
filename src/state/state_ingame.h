#ifndef STATE_IN_GAME_H
#define	STATE_IN_GAME_H

#include "src/world.h"
#include "state.h"

class StateInGame : public GameState {
private:
	World* world;

public:
	StateInGame();
	~StateInGame();

	void poll_input();
	void update();
};

#endif
