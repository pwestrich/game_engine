
#include "GUIManager.h"
#include "RenderManager.h"

using namespace std;

GUIManager::GUIManager(RenderManager *rm){

	assert(rm != NULL);

	renderManager = rm;
	gui = NULL;
	ogrePlatform = NULL;

}

GUIManager::~GUIManager(){} //nothing here

void GUIManager::loadResourceGroup(const string &groupName){



}

void GUIManager::unloadResourceGroup(){



}

void GUIManager::buildGUIFromXML(const string &filename){


	
}

