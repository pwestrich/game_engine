
#ifndef RESOURCE_MANAGER
#define RESOURCE_MANAGER

#include <string>

using namespace std;

class GameManager;

class ResourceManager {

private:

	GameManager *gameManager;

public:

	ResourceManager(GameManager *gm);
	~ResourceManager();

	//loads resources from the given XML files
	void loadResourcesFromXML(const string &filename);

};

#endif
