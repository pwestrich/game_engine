
#ifndef PATH_RESOURCE
#define PATH_RESOURCE

#include "GameResource.h"
#include "GameManager.h"

using namespace std;

class GameManager;

class PathResource : public GameResource {

private:



public:

	PathResource(uint32_t newID, const string &newGroup, const string &newName, GameManager *gm);
	~PathResource();

	//specific to PathResource
	string getPath();

	//must be implemented from GameResource
	void load();
	void unload();

};

#endif
