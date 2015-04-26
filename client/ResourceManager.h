
#ifndef RESOURCE_MANAGER
#define RESOURCE_MANAGER

#include <string>
#include <sstream>
#include <map>
#include <vector>

using namespace std;

class GameManager;
class GameResource;
struct AudioResourceInfo;

class ResourceManager {

private:

	//reference to the game manager for communication
	GameManager *gameManager;

	//the currently loaded group
	string groupLoaded;

	map<string, vector<GameResource*>> resources;

public:

	ResourceManager(GameManager *gm);
	~ResourceManager();

	//loads resources from the given XML files
	void loadResourcesFromXML(const string &filename);

	void loadGroup(const string &groupName);

	//unloads the currently loaded group
	void unloadResources();

	GameResource *getResourceByID(uint32_t id);

};

#endif
