
#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

//#include <map>
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
class PhysicsManager;

using namespace std;

class RenderManager : public InputListener {

private:

	//keep a copy of the game manager
	GameManager 	*gameManager;
    RenderListener 	*renderListener;
    GUIManager 		*guiManager;
    PhysicsManager 	*physicsManager;

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
    float wheelRotateAmount;
    Ogre::Vector3 cameraMovement;
    Ogre::Vector3 truckMovement;

    //come back here after physics
    //map<string, Vector3> velocityMap;

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

	//methods to alter the scene
	void rotateNode(const string &nodeName, const float w, const float x, const float y, const float z);
	void translateNode(const string &nodeName, const float x, const float y, const float z);
	void scaleNode(const string &nodeName, const float x, const float y, const float z);

	void setRotation(const string &nodeName, const float w, const float x, const float y, const float z);
	void setTranslation(const string &nodeName, const float x, const float y, const float z);
	void setScale(const string &nodeName, const float x, const float y, const float z);

	bool getRotation(const string &nodeName, float &w, float &x, float &y, float &z);
	bool getTranslation(const string &nodeName, float &x, float &y, float &z);
	bool getScale(const string &nodeName, float &x, float &y, float &z);

	void applyTorque(const string &name, const float x, const float y, const float z);
	void applyForce(const string &name, const float x, const float y, const float z);

	//methods to make a manual object
	void createManualObject(const string &name, const string &materialName);
	void clearManualObject(const string &name);
	void drawLine(const string &name, const string &material, const float x, const float y, const float z, 
					const float dx, const float dy, const float dz, const float r, const float g, const float b);

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
    void updatePhysics(const float timeStep);
    void updateAudio(const float timeStep);

    void playAudioByID(const uint32_t id, const int numRepeats);

	//methods to log things (pass to GameManager)
	void logInfo(const string &message);
	void logWarn(const string &message);
	void logDebug(const string &message);
	void logFatal(const string &message, const int line, const char *file);

	//methods to to script stuff (also goes to GameManager)
	void execute(const string &filename);
	void writeInt(const string &name, const int value);
	void writeFloat(const string &name, const float value);
	void writeString(const string &name, const string &value);

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
