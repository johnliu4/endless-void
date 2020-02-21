#ifndef GAME_STATE_H
#define GAME_STATE_H

class GameState {
public:
	virtual ~GameState() {};
	virtual void poll_input() = 0;
	virtual void update() = 0;
};

#endif
