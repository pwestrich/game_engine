
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

void GameManager::loadResourcesFromXML(const string &filename, const string &group){

	renderManager->loadResourcesFromXML(filename, group);

}

void GameManager::buildSceneFromXML(const string &filename){

	//do it manuallt for now
	//renderManager->buildSceneManually();
	//renderManager->buildSceneFromXML(filename);

}

void GameManager::startRendering(){

	renderManager->startRendering();

}

void GameManager::stopRendering(){

	renderManager->stopRendering();

}
