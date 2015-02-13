
#include <iostream>

#include "GameManager.h"

using namespace std;

int main(int argc, char **argv){

	GameManager *manager = GameManager::getGameManager();

	manager->loadResourcesFromXML("./xml/resources.xml", "0");
	manager->buildSceneFromXML("nope");
	manager->startRendering();

	cout << "Worked." << endl;

	manager->stopRendering();
	
	return 0;

}
