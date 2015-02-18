
#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <string>

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

	//methods to make the RenderManager do things
	void loadResourcesFromXML(const std::string &filename, const std::string &group);
	void buildSceneFromXML(const std::string &filename, const std::string &group);

	void buildSceneManually();

	void startRendering();
	void stopRendering();

};

#endif
