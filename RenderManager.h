
#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include <string>
#include <vector>

#include "Ogre.h"
#include "tinyxml.h"
#include "InputListener.h"
#include "utilities.h"

class GameManager;
class RenderListener;

class RenderManager {

private:

    //axis vectors
    static Ogre::Vector3 xAxis;
    static Ogre::Vector3 yAxis;
    static Ogre::Vector3 zAxis;

	//keep a copy of the game manager
	GameManager *gameManager;
    RenderListener *renderListener;

	Ogre::Root *root;
	Ogre::RenderWindow *window;
    Ogre::SceneManager* sceneManager;

    Ogre::Camera *camera;
    Ogre::Viewport *viewport;

    size_t windowHandle;
    Ogre::Real frameTimeElapsed;

    vector<Ogre::AnimationState*> animationStates;

    void createNodes(Ogre::SceneNode *parent, TiXmlNode *nodeTree);
    void createAnimation(Ogre::SceneNode *node, TiXmlNode *nodeTree);

public:

	//constructors and destructors
	RenderManager(GameManager *gman);
	~RenderManager();

	//a few getter methods
	size_t getRenderWindowHandle();
	size_t getWindowWidth();
	size_t getWindowHeight();

	Ogre::RenderWindow *getRenderWindow();
	Ogre::SceneManager *getSceneManager();

	//the setter methods
	void setTimeSinceLastFrame(Ogre::Real timeElapsed);

	//methods for adding resources to Ogre's resource manager
	void addPathResource(const string &path, const string &pathType, const string &group);
	void addMeshResource(const string &mesh, const string &type, const string &group);

	void initResourceGroup(const string &group);
	void loadResourceGroup(const string &group);
	void unloadResourceGroup(const string &group);

	//and actions
    void processAnimations(const float timeStep);
    void checkForInput(const float timeStep);

	void startRendering();
	void stopRendering();

	void buildSceneFromXML(const std::string &filename, const string &sceneName);
	void buildSceneManually();

	//methods to alter the scene based on input
	void mouseMoved(const uint32_t x, const uint32_t y, const int32_t dx, const int32_t dy);
	void keyPressed(const KeyboardKey key);
	void keyReleased(const KeyboardKey key);

};

#endif
