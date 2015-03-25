
#include <iostream>

#include "GameManager.h"

using namespace std;

int main(int argc, char **argv){

	GameManager *manager = GameManager::getGameManager();

	manager->loadResourcesFromXML("./xml/resources.xml");
	manager->loadResources("0");
	manager->buildSceneFromXML("./xml/scene.xml", "0");
	manager->startRendering();

	delete manager;
	
	return 0;

}
