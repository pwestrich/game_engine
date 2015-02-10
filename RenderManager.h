
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

    //build a scene manually, for testing stuff


public:

	//constructors and destructors
	RenderManager(GameManager *gman);
	virtual ~RenderManager();

	//a ton of getter methods
	size_t getRenderWindowHandle();

	int getRenderWindowWidth();
	int getRenderWindowHeight();

	Ogre::RenderWindow *getRenderWindow();
	Ogre::SceneManager *getSceneManager();

	//the setter methods
	void setTimeSinceLastFrame(Ogre::Real timeElapsed);

	//and actions
	void startRendering();
	void stopRendering();

	void loadResourcesFromXML(const std::string &filename);
	void buildSceneFromXML(const std::string &filename);

	void loadResourcesManually();
	void buildSceneManually();

};

#endif
