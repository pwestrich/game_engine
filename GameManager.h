
#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <string>

using namespace std;

//forward-declare the other managers
class RenderManager;
class LogManager;
class ResourceManager;

class GameManager {

private:

	//keep a copy of every manager
	RenderManager *renderManager;
	LogManager *logManager;
	ResourceManager *resourceManager;

	//only one copy at a time is allowed
	GameManager();

public:

	~GameManager();

	//return a pointer to the game manager
	static GameManager *getGameManager();

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

	//methods to log things
	void logInfo(const string &message);
	void logWarn(const string &message);
	void logDebug(const string &message);
	void logFatal(const string &message, const int line, const char *file);

	//methods to make the ResourceManager do things
	void loadResourcesFromXML(const std::string &filename, const std::string &group);
	void unloadResources();

};

#endif
