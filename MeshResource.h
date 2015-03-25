
#ifndef MESH_RESOURCE
#define MESH_RESOURCE

#include "GameResource.h"

using namespace std;

class GameManager;

class MeshResource : public GameResource {

private:

public:

	MeshResource(uint32_t newID, const string &newGroup, const string &newName, GameManager *gm);
	~MeshResource();

	//methods from GameResource
	void load();
	void unload();

};

#endif
