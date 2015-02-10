
#include "GameManager.h"
#include "RenderManager.h"

using namespace std;

GameManager::GameManager(){

	//create all the managers
	renderManager = new RenderManager(this);

	//load the resources
	renderManager->loadResourcesManually();

	//build scene
	renderManager->buildSceneManually();

	renderManager->startRendering();

}

GameManager::~GameManager(){

	//delete everything
	delete renderManager;

}

GameManager *GameManager::getGameManager(){

	static GameManager manager;
	return &manager;

}
