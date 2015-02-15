
#include <iostream>

#include "GameManager.h"

using namespace std;

int main(int argc, char **argv){

	GameManager *manager = GameManager::getGameManager();

	manager->loadResourcesFromXML("./xml/resources.xml", "1");
	manager->buildSceneFromXML("./xml/scene.xml", "1");
	manager->startRendering();
	
	return 0;

}
