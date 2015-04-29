
#include "GameManager.h"
#include "LogManager.h"
#include "RenderManager.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "AudioManager.h"
#include "GameResource.h"
#include "AudioResource.h"
#include "ScriptManager.h"
#include "NetworkManager.h"

//I'm not happy with this, but it's what I have to do
#include "LuaContext.hpp"

using namespace std;

GameManager::GameManager(){

	//init my one variable
	currentBackgroundMusic = 0;

	//create all the managers
	logManager 		= new LogManager(this);
	renderManager 	= new RenderManager(this);
	resourceManager = new ResourceManager(this);
	inputManager 	= new InputManager(this);
	audioManager 	= new AudioManager(this);
	scriptManager 	= new ScriptManager(this);
	networkManager 	= new NetworkManager(this, "localhost", 12345); //use command line args later?

	//set input listeners
	inputManager->addListener(this);
	inputManager->addListener(renderManager);

	//load resources and build the scene
	loadResourcesFromXML("./assets/xml/resources.xml");
	loadResources("0");
	buildSceneFromXML("./assets/xml/scene.xml", "0");
	
	startAudio();
	setVolume(0.40);

	//register various functions with the ScriptManager so that lua scripts can use them
	//I'm not happy witht he class cohesino going on here, but it won't work otherwise
	LuaContext *lua = scriptManager->getLuaContext();

	//GameManager stuff
	lua->registerFunction("playAudioByID", &GameManager::playAudioByID);
	lua->registerFunction("stopAudioByID", &GameManager::stopAudioByID);
	lua->registerFunction("setBackgroundMusic", &GameManager::setBackgroundMusic);
	lua->registerFunction("setVolume", &GameManager::setVolume);
	lua->registerFunction("logInfo", &GameManager::logInfo);
	lua->registerFunction("logWarn", &GameManager::logWarn);
	lua->registerFunction("logDebug", &GameManager::logDebug);
	lua->writeVariable("Game", this);

	//scene stuff
	lua->registerFunction("rotateNode", &RenderManager::rotateNode);
	lua->registerFunction("translateNode", &RenderManager::translateNode);
	lua->registerFunction("scaleNode", &RenderManager::scaleNode);
	lua->registerFunction("orientNode", &RenderManager::setRotation);
	lua->registerFunction("positionNode", &RenderManager::setTranslation);
	lua->registerFunction("setScale", &RenderManager::setScale);

	//camera stuff
	lua->registerFunction("translateCamera", &RenderManager::translateCamera);
	lua->registerFunction("rotateCamera", &RenderManager::rotateCamera);

	//physics stuff
	lua->registerFunction("applyForce", &RenderManager::applyForce);
	lua->registerFunction("applyTorque", &RenderManager::applyTorque);
	lua->registerFunction("setLinearVelocity", &RenderManager::setLinearVelocity);
	lua->registerFunction("setAngularVelocity", &RenderManager::setAngularVelocity);
	lua->writeVariable("Scene", renderManager);

	//run the startup script
	execute("./assets/lua/init.lua");

	networkManager->startListening();

	//start drawing
	startRendering();

}

GameManager::~GameManager(){

	//delete everything in the opposite order they were delcared
	delete networkManager;
	delete scriptManager;
	delete inputManager;
	delete resourceManager;
	delete renderManager;
	delete audioManager;
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

	} else {

		//send the key press over the network
		stringstream ss;
		ss << "Key:" << key;
		send(ss.str().c_str(), strlen(ss.str().c_str()));

		//determine what to do by means of script
		scriptManager->writeInt("keyPressed", static_cast<int>(key));
		scriptManager->execute("./assets/lua/input.lua");

	}

}

void GameManager::keyReleased(const KeyboardKey key){}

void GameManager::mouseMoved(const uint32_t x, const uint32_t y, const int32_t dx, const int32_t dy){

	//tell the render manager to deal with it and send it over the newtwork
	stringstream ss;
	ss << "Move:" << x << "," << y << "," << dx << "," << dy;
	networkManager->send(ss.str().c_str(), strlen(ss.str().c_str()));
	renderManager->mouseMoved(x, y, dx, dy);

}

void GameManager::mousePressed(const uint32_t x, const uint32_t y, const MouseButton button){

	//tell the render manager to do something with it
	stringstream ss;
	ss << "Click:" << x << "," << y << "," << button;
	networkManager->send(ss.str().c_str(), strlen(ss.str().c_str()));
	renderManager->mousePressed(x, y, button);

}

void GameManager::mouseReleased(const uint32_t x, const uint32_t y, const MouseButton button){
	
	stringstream ss;
	ss << "Release:" << x << "," << y << "," << button;
	networkManager->send(ss.str().c_str(), strlen(ss.str().c_str()));
	renderManager->mouseReleased(x, y, button);

}

void GameManager::joystickAxisMoved(const int *axes, const int numAxes){}

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
void GameManager::playAudioByID(const uint32_t id, const int numRepeats){

	AudioResource *music = static_cast<AudioResource*>(getResourceByID(id));

	if (music && music->getInfo()){

		playAudio(music->getInfo(), numRepeats, id);

	} else {

		logWarn("Attempt to play not-audio caught");

	}

}

void GameManager::stopAudioByID(const uint32_t id){

	audioManager->stopAudioByID(id);

}

void GameManager::setBackgroundMusic(const uint32_t id){

	if (currentBackgroundMusic != 0 && id != currentBackgroundMusic){

		stopAudioByID(currentBackgroundMusic);

	}

	playAudioByID(id, 25);
	currentBackgroundMusic = id;

}

void GameManager::playAudio(AudioResourceInfo *info, const int numRepeats, const uint32_t id){

	audioManager->playAudio(info, numRepeats, id);

}

void GameManager::updateAudio(const float timeStep){

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

void GameManager::loadAudioStream(const string &filename, AudioResourceInfo *info){

	audioManager->loadAudioStream(filename, info);

}

void GameManager::loadAudioSample(const string &filename, AudioResourceInfo *info){

	audioManager->loadAudioSample(filename, info);

}

void GameManager::unloadAudio(AudioResourceInfo *info){

	audioManager->unloadAudio(info);

}

AudioResourceInfo *GameManager::createAudioInfo(){

	return audioManager->createAudioInfo();

}

//methods to make the ScriptManager do things -----------------------------------------------------
void GameManager::execute(const string &filename){

	scriptManager->execute(filename);

}

void GameManager::writeInt(const string &name, const int value){

	scriptManager->writeInt(name, value);

}

void GameManager::writeFloat(const string &name, const float value){

	scriptManager->writeFloat(name, value);

}

void GameManager::writeString(const string &name, const string &value){

	scriptManager->writeString(name, value);

}

//methods to make the NetworkManager do things ----------------------------------------------------
void GameManager::send(const char *data, const int dataSize){

	networkManager->send(data, dataSize);

}

void GameManager::messageReceived(const char *message, const int messageLength){

	logManager->logInfo(string("Message received: ") + message);

}
