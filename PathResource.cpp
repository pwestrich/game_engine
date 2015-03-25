
#include "PathResource.h"

using namespace std;

PathResource::PathResource(uint32_t newID, const string &newGroup, const string &newName, GameManager *gm) 
							: GameResource(newID, newGroup, newName, gm, RT_PATH){

	//simply call the parent constructor; no need for more work here

}

PathResource::~PathResource(){} //nothing here

//specific to PathResource
string PathResource::getPath(){

	return getFilename();

}

//must be implemented from GameResource
void PathResource::load(){

	gameManager->addPathResource(getFilename(), "FileSystem", getGroup());
	loaded = true;

}

void PathResource::unload(){

	loaded = false;

}
