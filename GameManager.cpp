
#include "GameManager.h"
#include "RenderManager.h"

using namespace std;

GameManager::GameManager(){

	//create all the managers
	renderManager = new RenderManager(this);

}

GameManager::~GameManager(){

	//delete everything
	delete renderManager;

}

GameManager *GameManager::getGameManager(){

	static GameManager manager;
	return &manager;

}
