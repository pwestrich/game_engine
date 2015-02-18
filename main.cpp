
#include <iostream>

#include "GameManager.h"

using namespace std;

int main(int argc, char **argv){

	GameManager *manager = GameManager::getGameManager();

	manager->loadResourcesFromXML("./xml/resources.xml", "0");
	manager->buildSceneFromXML("./xml/scene.xml", "0");
	//manager->buildSceneManually();
	manager->startRendering();
	
	return 0;

}
