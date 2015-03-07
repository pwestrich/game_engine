
#include "GameManager.h"
#include "LogManager.h"
#include "RenderManager.h"
#include "ResourceManager.h"
#include "InputManager.h"

using namespace std;

GameManager::GameManager(){

	//create all the managers
	logManager = new LogManager(this);
	renderManager = new RenderManager(this);
	resourceManager = new ResourceManager(this);
	inputManager = new InputManager(this);

}

GameManager::~GameManager(){

	unloadResources();

	//delete everything
	delete inputManager;
	delete resourceManager;
	delete renderManager;
	delete logManager;

}

GameManager *GameManager::getGameManager(){

	static GameManager manager;
	return &manager;

}

//InputListener functions -------------------------------------------------------------------------
void GameManager::keyPressed(const KeyboardKey key){

	if (key == KB_ESC){

		renderManager->stopRendering();

	}

}

void GameManager::keyReleased(const KeyboardKey key){}

void GameManager::mouseMoved(const uint32_t x, const uint32_t y, const uint32_t dx, const uint32_t dy){}

void GameManager::mousePressed(const uint32_t x, const uint32_t y, const MouseButton button){}

void GameManager::mouseReleased(const uint32_t x, const uint32_t y, const MouseButton button){}

void GameManager::joystickAxisMoved(const JoystickAxis axis, const uint32_t amount){}

void GameManager::joystickButtonPressed(const JoystickButton button){}

void GameManager::checkForInput(const float timeStep){

	inputManager->checkForInput(timeStep);

}

//RenderManager methods ---------------------------------------------------------------------------
void GameManager::buildSceneFromXML(const string &filename, const string &group){

	renderManager->buildSceneFromXML(filename, group);

}

void GameManager::buildSceneManually(){

	renderManager->buildSceneManually();

}

void GameManager::addPathResource(const string &path, const string &pathType, const string &group){

	renderManager->addPathResource(path, pathType, group);

}
void GameManager::addMeshResource(const string &mesh, const string &type, const string &group){

	renderManager->addMeshResource(mesh, type, group);

}

void GameManager::initResourceGroup(const string &group){

	renderManager->initResourceGroup(group);

}

void GameManager::loadResourceGroup(const string &group){

	renderManager->loadResourceGroup(group);

}

void GameManager::unloadResourceGroup(const string &group){

	renderManager->unloadResourceGroup(group);

}

void GameManager::startRendering(){

	renderManager->startRendering();

}

void GameManager::stopRendering(){

	renderManager->stopRendering();

}

size_t GameManager::getWindowHandle(){

	return renderManager->getRenderWindowHandle();

}

//log manager methods -----------------------------------------------------------------------------
void GameManager::logInfo(const string &message){

	logManager->logInfo(message);

}

void GameManager::logWarn(const string &message){

	logManager->logWarning(message);

}

void GameManager::logDebug(const string &message){

	logManager->logDebug(message);

}

void GameManager::logFatal(const string &message, const int line, const char *file){

	logManager->logFatal(message, line, file);

}

//resource manager methods ------------------------------------------------------------------------
void GameManager::loadResourcesFromXML(const string &filename, const string &group){

	//render manager still does this for now
	resourceManager->loadResourcesFromXML(filename, group);

}

void GameManager::unloadResources(){

	resourceManager->unloadResources();

}
