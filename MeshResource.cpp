
#include "MeshResource.h"
#include "GameManager.h"

using namespace std;

MeshResource::MeshResource(uint32_t newID, const string &newGroup, const string &newName, GameManager *gm)
				: GameResource(newID, newGroup, newName, gm, RT_MESH){

}

MeshResource::~MeshResource(){} //nothing here

//methods from GameResource
void MeshResource::load(){

	gameManager->addMeshResource(getFilename(), "Mesh", getGroup());
	loaded = true;

}

void MeshResource::unload(){

	loaded = false;

}
