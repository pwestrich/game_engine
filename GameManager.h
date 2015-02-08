
#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

using namespace std;

//forward-declare the other managers
class RenderManager;

class GameManager {

private:

	//keep a copy of every manager
	RenderManager *renderManager;

	//only one copy at a time is allowed
	GameManager();

public:

	virtual ~GameManager();

	//return a pointer to the game manager
	static GameManager *getGameManager();

};

#endif
