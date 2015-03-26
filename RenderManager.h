
#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include <string>
#include <vector>

#include "Ogre.h"
#include "tinyxml.h"
#include "InputListener.h"
#include "utilities.h"

//forward-declare these classes so the compiler is happy
class GameManager;
class RenderListener;
class GUIManager;

//enumeration for the wheel's state
enum WheelState { WS_LEFT = 0, WS_FORWARD = 1, WS_RIGHT = 2 };

//enumeration for the truck's state
enum TruckState { TS_STILL, TS_FORWARD, TS_BACKWARD };

class RenderManager : public InputListener {

private:

	//keep a copy of the game manager
	GameManager *gameManager;
   RenderListener *renderListener;
   GUIManager *guiManager;

   //Ogre scene stuff
	Ogre::Root *root;
	Ogre::RenderWindow *window;
   Ogre::SceneManager* sceneManager;

   //the camera, viewport, and window we're rendering in
   Ogre::Camera *camera;
   Ogre::Viewport *viewport;
   size_t windowHandle;

   //the animation states andthe time since last frame
   Ogre::Real frameTimeElapsed;
   vector<Ogre::AnimationState*> animationStates;

   //variables for keeping track of movement
   int wheelState;
   TruckState truckState;

   bool cameraLocked;

   Ogre::Vector3 cameraMovement;
   Ogre::Vector3 truckMovement;

   //functions to recursivley generate the scene graph from XML
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
    void updateMovement(const float timeStep);
    void updateAudio(const float timeStep);

	//methods to log things (pass to GameManager)
	void logInfo(const string &message);
	void logWarn(const string &message);
	void logDebug(const string &message);
	void logFatal(const string &message, const int line, const char *file);

	void startRendering();
	void stopRendering();

	//methods to alter the scene based on input
	void keyPressed(const KeyboardKey key);
	void keyReleased(const KeyboardKey key);

	void mouseMoved(const uint32_t x, const uint32_t y, const int32_t dx, const int32_t dy);
	void mousePressed(const uint32_t x, const uint32_t y, const MouseButton button);
	void mouseReleased(const uint32_t x, const uint32_t y, const MouseButton button);

	void joystickAxisMoved(const JoystickAxis axis, const uint32_t amount);
	void joystickButtonPressed(const JoystickButton button);

	//methods to build the scene
	void buildSceneFromXML(const std::string &filename, const string &sceneName);
	void buildSceneManually();

};

#endif
