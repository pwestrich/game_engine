
#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include "Ogre.h"
#include "tinyxml.h"

class GameManager;

class RenderManager {

private:

	//keep a copy of the game manager
	GameManager *gameManager;

	Ogre::Root *root;
	Ogre::RenderWindow *window;
    Ogre::SceneManager* sceneManager;
    Ogre::Camera* camera;
    Ogre::Viewport* viewport;

    size_t windowHandle;
    Ogre::Real frameTimeElapsed;

    std::string groupLoaded;

public:

	RenderManager(GameManager *gman);
	virtual ~RenderManager();

	//build a scene manually, for testing stuff
	void buildSceneManually();

};

#endif
