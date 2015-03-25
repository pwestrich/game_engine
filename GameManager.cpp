
#include "GameManager.h"
#include "LogManager.h"
#include "RenderManager.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "AudioManager.h"

using namespace std;

GameManager::GameManager(){

	//create all the managers
	logManager = new LogManager(this);
	renderManager = new RenderManager(this);
	resourceManager = new ResourceManager(this);
	inputManager = new InputManager(this);
	audioManager = new AudioManager(this);

	//set input listeners
	inputManager->addListener(this);
	inputManager->addListener(renderManager);

}

GameManager::~GameManager(){

	//delete everything
	delete inputManager;
	delete renderManager;
	delete resourceManager;
	delete logManager;

}

GameManager *GameManager::getGameManager(){

	static GameManager manager;
	return &manager;

}

//InputListener functions -------------------------------------------------------------------------
void GameManager::keyPressed(const KeyboardKey key){

	//excape will quit the game
	if (key == KB_ESC){

		logManager->logInfo("Stopping render process...");
		renderManager->stopRendering();

		logManager->logInfo("Stopped.");
		logManager->logInfo("Exiting program...");
		exit(EXIT_SUCCESS);

	}

}

void GameManager::keyReleased(const KeyboardKey key){}

void GameManager::mouseMoved(const uint32_t x, const uint32_t y, const int32_t dx, const int32_t dy){

	//tell the render manager to deal with it
	//mouse movement should rotate the camera
	renderManager->mouseMoved(x, y, dx, dy);

}

void GameManager::mousePressed(const uint32_t x, const uint32_t y, const MouseButton button){

	//tell the render manager to do something with it
	renderManager->mousePressed(x, y, button);

}

void GameManager::mouseReleased(const uint32_t x, const uint32_t y, const MouseButton button){}

void GameManager::joystickAxisMoved(const JoystickAxis axis, const uint32_t amount){}

void GameManager::joystickButtonPressed(const JoystickButton button){}

void GameManager::checkForInput(const float timeStep){

	//message from the render listener each frame
	//tell the input manager to check for input
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

size_t GameManager::getWindowWidth(){

	return renderManager->getWindowWidth();

}

size_t GameManager::getWindowHeight(){

	return renderManager->getWindowHeight();

}

//log manager methods -----------------------------------------------------------------------------
//pretty much all these methods pass the info along to the log manager
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
void GameManager::loadResourcesFromXML(const string &filename){

	//render manager still does this for now
	resourceManager->loadResourcesFromXML(filename);

}

void GameManager::unloadResources(){

	resourceManager->unloadResources();

}

void GameManager::loadResources(const string &group){

	resourceManager->loadGroup(group);

}

GameResource *GameManager::getResourceByID(uint32_t id){

	return resourceManager->getResourceByID(id);

}

//methods to tell the AudioManager to do things
void GameManager::playAudio(AudioResourceInfo *info, const int numRepeats){

	audioManager->playAudio(info, numRepeats);

}

void GameManager::updateAudio(){

	audioManager->updateAudio();

}

void GameManager::pauseAudio(){

	audioManager->pause();

}

void GameManager::startAudio(){

	audioManager->start();

}

void GameManager::setVolume(const float volume){

	audioManager->setVolume(volume);

}

void GameManager::loadAudio(const string &filename, AudioResourceInfo *info){

	audioManager->loadAudio(filename, info);

}

void GameManager::unloadAudio(AudioResourceInfo *info){

	audioManager->unloadAudio(info);

}

AudioResourceInfo *GameManager::createAudioInfo(){

	return audioManager->createAudioInfo();

}
