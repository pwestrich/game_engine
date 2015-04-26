
#ifndef GAME_RESOURCE
#define GAME_RESOURCE

#include <cstdlib>
#include <cassert>
#include <string>

using namespace std;

class GameManager;

enum ResourceType : uint8_t { RT_PATH, RT_MESH, RT_AUDIO };

class GameResource {

private:

	//these won't ever be modified once set
	string group;
	string filename;
	uint32_t id;
	ResourceType type;

protected:

	GameManager *gameManager;
	bool loaded;

public:

	GameResource(uint32_t newID, const string &newGroup, const string &newName, GameManager *gm, ResourceType newType);
	virtual ~GameResource();

	string getGroup();
	string getFilename();
	uint32_t getResourceID();
	bool isLoaded();

	//resources must implement these methods
	virtual void load() = 0;
	virtual void unload() = 0;

};

#endif
