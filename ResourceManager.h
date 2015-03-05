
#ifndef RESOURCE_MANAGER
#define RESOURCE_MANAGER

#include <string>

#include "tinyxml.h"	//to read in from XML

using namespace std;

class GameManager;

class ResourceManager {

private:

	//reference to the game manager for communication
	GameManager *gameManager;

	//the currently loaded group
	string groupLoaded;

public:

	ResourceManager(GameManager *gm);
	~ResourceManager();

	//loads resources from the given XML files
	void loadResourcesFromXML(const string &filename, const string &group_name);

	//unloads the currently loaded group
	void unloadResources();

};

#endif
