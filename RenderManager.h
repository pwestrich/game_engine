
#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include "Ogre.h"
#include "tinyxml.h"

class GameManager;

class RenderManager {

private:

	//keep a copy of the game manager
	GameManager *gameManager;

public:

	RenderManager(GameManager *gman);
	virtual ~RenderManager();

};

#endif
