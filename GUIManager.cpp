
#include "GUIManager.h"
#include "RenderManager.h"

using namespace std;

GUIManager::GUIManager(RenderManager *rm){

	assert(rm != NULL);

	renderManager = rm;
	gui = NULL;
	ogrePlatform = NULL;

}

GUIManager::~GUIManager(){} //nothing here

void GUIManager::loadResourceGroup(const string &groupName){

	ogrePlatform = new MyGUI::OgrePlatform();
	ogrePlatform->initialise(renderManager->getRenderWindow(), renderManager->getSceneManager(), groupName);

	gui = new MyGUI::Gui();
	gui->initialise();

	MyGUI::ResourceManager& myGUIResourceManager = MyGUI::Singleton<MyGUI::ResourceManager>::getInstance();
	myGUIResourceManager.load("MyGUI_Core.xml");

	try {

		buildGUIFromXML("./xml/gui.xml");

	} catch (MyGUI::Exception &it) {

		renderManager->logWarn(it.what());

	}

}

void GUIManager::unloadResourceGroup(){

	gui->destroyAllChildWidget();

	MyGUI::ResourceManager& myGUIResourceManager = MyGUI::Singleton<MyGUI::ResourceManager>::getInstance();
	myGUIResourceManager.clear();

	gui->shutdown();
	delete gui;
	gui = NULL;

	ogrePlatform->shutdown();
	delete ogrePlatform;
	ogrePlatform = NULL;

}

void GUIManager::buildGUIFromXML(const string &filename){



}

