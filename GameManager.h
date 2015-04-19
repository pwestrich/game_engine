
#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <cassert>
#include <string>

#include "InputListener.h"

using namespace std;

//forward-declare the other managers
class RenderManager;
class LogManager;
class ResourceManager;
class InputManager;
class AudioManager;
class GameResource;
class ScriptManager;

struct AudioResourceInfo;

class GameManager : public InputListener {

private:

	//keep a copy of every manager
	RenderManager 	*renderManager;
	LogManager 		*logManager;
	ResourceManager *resourceManager;
	InputManager 	*inputManager;
	AudioManager 	*audioManager;
	ScriptManager 	*scriptManager;

	uint32_t currentBackgroundMusic;

	//only one copy at a time is allowed
	GameManager();

public:

	~GameManager();

	//return a pointer to the game manager
	static GameManager *getGameManager();

	//methods to be an input listnener
	void keyPressed(const KeyboardKey key);
	void keyReleased(const KeyboardKey key);

	void mouseMoved(const uint32_t x, const uint32_t y, const int32_t dx, const int32_t dy);
	void mousePressed(const uint32_t x, const uint32_t y, const MouseButton button);
	void mouseReleased(const uint32_t x, const uint32_t y, const MouseButton button);

	void joystickAxisMoved(const JoystickAxis axis, const uint32_t amount);
	void joystickButtonPressed(const JoystickButton button);

	//InputManager methods
	void checkForInput(const float timeStep);

	//methods to make the RenderManager do things
	void buildSceneFromXML(const std::string &filename, const std::string &group);
	void buildSceneManually();

	void addPathResource(const string &path, const string &pathType, const string &group);
	void addMeshResource(const string &mesh, const string &type, const string &group);

	void initResourceGroup(const string &group);
	void loadResourceGroup(const string &group);
	void unloadResourceGroup(const string &group);

	void startRendering();
	void stopRendering();

	size_t getWindowHandle();
	size_t getWindowWidth();
	size_t getWindowHeight();

	//methods to log things
	void logInfo(const string &message);
	void logWarn(const string &message);
	void logDebug(const string &message);
	void logFatal(const string &message, const int line, const char *file);

	//methods to make the ResourceManager do things
	void loadResourcesFromXML(const std::string &filename);
	void unloadResources();

	void loadResources(const string &group);
	GameResource *getResourceByID(uint32_t id);

	//methods to tell the AudioManager to do things
	void playAudioByID(const uint32_t id, const int numRepeats);
	void stopAudioByID(const uint32_t id);
	void setBackgroundMusic(const uint32_t id);
	void playAudio(AudioResourceInfo *info, const int numRepeats, const uint32_t id);
	void updateAudio(const float timeStep);

	void pauseAudio();
	void startAudio();

	void setVolume(const float volume);

	void loadAudioStream(const string &filename, AudioResourceInfo *info);
	void loadAudioSample(const string &filename, AudioResourceInfo *info);
	void unloadAudio(AudioResourceInfo *info);

	AudioResourceInfo *createAudioInfo();

	//methods to make the ScriptMAnager do things
	void execute(const string &filename);
	void writeInt(const string &name, const int value);
	void writeFloat(const string &name, const float value);
	void writeString(const string &name, const string &value);

};

#endif
