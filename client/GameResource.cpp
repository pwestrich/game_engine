
#include "GameResource.h"
#include "GameManager.h"

using namespace std;

GameResource::GameResource(uint32_t newID, const string &newGroup, const string &newName, GameManager *gm, ResourceType newType){

	assert(gm != NULL);

	id = newID;
	group = newGroup;
	filename = newName;
	gameManager = gm;
	type = newType;

}

GameResource::~GameResource(){



}

string GameResource::getGroup(){

	return group;

}

string GameResource::getFilename(){

	return filename;

}

uint32_t GameResource::getResourceID(){

	return id;

}

bool GameResource::isLoaded(){

	return loaded;

}
