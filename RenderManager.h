
#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include <string>
#include <vector>

#include "Ogre.h"
#include "tinyxml.h"
#include "utilities.h"

class GameManager;
class RenderListener;

class RenderManager {

private:

	//keep a copy of the game manager
	GameManager *gameManager;

   RenderListener *renderListener;

	Ogre::Root *root;
	Ogre::RenderWindow *window;
   Ogre::SceneManager* sceneManager;

   size_t windowHandle;
   Ogre::Real frameTimeElapsed;

   std::string groupLoaded;

   vector<Ogre::AnimationState*> animationStates;

   void createNodes(Ogre::SceneNode *parent, TiXmlNode *nodeTree);
   void createAnimation(Ogre::SceneNode *node, TiXmlNode *nodeTree);

public:

	//constructors and destructors
	RenderManager(GameManager *gman);
	virtual ~RenderManager();

	//a ton of getter methods
	size_t getRenderWindowHandle();

	Ogre::RenderWindow *getRenderWindow();
	Ogre::SceneManager *getSceneManager();

	//the setter methods
	void setTimeSinceLastFrame(Ogre::Real timeElapsed);

	//and actions

   void processAnimations(const float timeStep);

	void startRendering();
	void stopRendering();

	void loadResourcesFromXML(const std::string &filename, const std::string &group_name);
	void buildSceneFromXML(const std::string &filename, const string &sceneName);

	void unloadResources();

	void buildSceneManually();

};

#endif
