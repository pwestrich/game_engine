
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

void GUIManager::loadResourceGroup(const string &filename, const string &groupName){

	renderManager->logInfo("Loading GUI...");

	try {

		ogrePlatform = new MyGUI::OgrePlatform();
		ogrePlatform->initialise(renderManager->getRenderWindow(), renderManager->getSceneManager(), groupName);

		gui = new MyGUI::Gui();
		gui->initialise();

		MyGUI::ResourceManager& myGUIResourceManager = MyGUI::Singleton<MyGUI::ResourceManager>::getInstance();
		myGUIResourceManager.load("MyGUI_Core.xml");

		buildGUIFromXML(filename);

	} catch (MyGUI::Exception &it){

		renderManager->logWarn(it.what());

	} catch (Ogre::Exception &it){

		renderManager->logFatal(it.what(), __LINE__, __FILE__);

	} catch (...){

		renderManager->logFatal("Unexpected error while initalizing GUI.", __LINE__, __FILE__);

	}

	renderManager->logInfo("GUI built.");

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

	renderManager->logInfo("Building GUI...");

	int values[5] = {0,0,0,0,0};
	TiXmlDocument doc(filename.c_str());

	if (doc.LoadFile()){

		TiXmlNode *guiNode = doc.FirstChild("gui");

		if (guiNode){

			TiXmlNode *windowsNode = guiNode->FirstChild("windows");

			if (windowsNode){

				for (TiXmlNode *window = windowsNode->FirstChild(); window; window = window->NextSibling()){

					TiXmlElement *windowElement = static_cast<TiXmlElement*>(window->FirstChild("name"));
					string name = windowElement->GetText();

					windowElement = static_cast<TiXmlElement*>(window->FirstChild("caption"));
					string caption = windowElement->GetText();

					windowElement = static_cast<TiXmlElement*>(window->FirstChild("skin"));
					string skin = windowElement->GetText();

					windowElement = static_cast<TiXmlElement*>(window->FirstChild("position"));
					string position = windowElement->GetText();

					windowElement = static_cast<TiXmlElement*>(window->FirstChild("size"));
					string size = windowElement->GetText();

					windowElement = static_cast<TiXmlElement*>(window->FirstChild("layer"));
					string layer = windowElement->GetText();

					parseInts(position, values);
					parseInts(size, values + 2);

					MyGUI::Window *win = gui->createWidget<MyGUI::Window>(skin, values[0], values[1], values[2], values[3], MyGUI::Align::Default, layer, name);
					win->setCaption(caption);

					TiXmlNode *buttonsNode = window->FirstChild("buttons");

					if (buttonsNode){

						for (TiXmlNode *button = buttonsNode->FirstChild(); button; button = button->NextSibling()){

							windowElement = static_cast<TiXmlElement*>(button->FirstChild("name"));
							name = windowElement->GetText();

							windowElement = static_cast<TiXmlElement*>(button->FirstChild("caption"));
							caption = windowElement->GetText();

							windowElement = static_cast<TiXmlElement*>(button->FirstChild("skin"));
							skin = windowElement->GetText();

							windowElement = static_cast<TiXmlElement*>(button->FirstChild("position"));
							position = windowElement->GetText();

							windowElement = static_cast<TiXmlElement*>(button->FirstChild("size"));
							size = windowElement->GetText();

							windowElement = static_cast<TiXmlElement*>(button->FirstChild("font"));
							string font = windowElement->GetText();
							
							parseInts(position, values);
							parseInts(size, values + 2);
							parseInts(font, values + 4);

							MyGUI::Button *b = win->createWidget<MyGUI::Button>(skin, values[0], values[1], values[2], values[3], MyGUI::Align::Default, name);
							b->setCaption(caption);
							b->setFontHeight(values[4]);
							b->setTextColour(MyGUI::Colour(0,0,0));
							//b->eventMouseButtonPressed += newDelegate(this, &GUIManager::buttonPressed);

						}

					} else {

						renderManager->logInfo("No buttons in this GUI.");

					}

					TiXmlNode *combosNode = window->FirstChild("combos");

					if (combosNode){



					} else {

						renderManager->logInfo("No combo boxes in this GUI.");

					}

				}

			} else {

				renderManager->logFatal("Invalid GUI XML format: missing <windows>", __LINE__, __FILE__);

			}

		} else {

			renderManager->logWarn("Should there be a GUI in this scene?");

		}

	} else {

		renderManager->logFatal("Error while loading GUI XML file.", __LINE__, __FILE__);

	}

}

