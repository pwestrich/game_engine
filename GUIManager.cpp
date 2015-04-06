
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

//GUI Listener methods ----------------------------------------------------------------------------
void GUIManager::buttonPressed(MyGUI::Widget *sender, int left, int top, MyGUI::MouseButton id){

	MyGUI::Button *button = static_cast<MyGUI::Button*>(sender);

	try {

		string script = scriptMap.at(button);

		renderManager->writeString("buttonName", button->getName());
		renderManager->execute(script);

	} catch (out_of_range &it){

		renderManager->logInfo("No script for this button.");

	}

}

void GUIManager::scrollBarMoved(MyGUI::Widget *sender, int left, int top, MyGUI::MouseButton id){

	MyGUI::ScrollBar *bar = static_cast<MyGUI::ScrollBar*>(sender);

	try {

		string script = scriptMap.at(bar);

		renderManager->writeString("scrollBarName", bar->getName());
		renderManager->writeInt("position", bar->getScrollPosition());
		renderManager->execute(script);

	} catch (out_of_range &it){

		renderManager->logInfo("No script for this scroll bar.");

	}

}

//InputListener methods ---------------------------------------------------------------------------
void GUIManager::keyPressed(const KeyboardKey key){}
void GUIManager::keyReleased(const KeyboardKey key){}

void GUIManager::mouseMoved(const uint32_t x, const uint32_t y, const int32_t dx, const int32_t dy){

	MyGUI::InputManager& inputManager = MyGUI::Singleton<MyGUI::InputManager>::getInstance();
	inputManager.injectMouseMove(x, y, 0);

}

void GUIManager::mousePressed(const uint32_t x, const uint32_t y, const MouseButton button){

	MyGUI::InputManager& inputManager = MyGUI::Singleton<MyGUI::InputManager>::getInstance();
	inputManager.injectMousePress(x, y, MyGUI::MouseButton::Enum(button));

}

void GUIManager::mouseReleased(const uint32_t x, const uint32_t y, const MouseButton button){

	MyGUI::InputManager& inputManager = MyGUI::Singleton<MyGUI::InputManager>::getInstance();
	inputManager.injectMouseRelease(x, y, MyGUI::MouseButton::Enum(button));

}

void GUIManager::joystickAxisMoved(const JoystickAxis axis, const uint32_t amount){}
void GUIManager::joystickButtonPressed(const JoystickButton button){}

//private methods below here ----------------------------------------------------------------------
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
					win->setVisible(true);

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

							TiXmlNode *scrollBar = button->FirstChild("scrollBar");

							if (scrollBar){

								windowElement = static_cast<TiXmlElement*>(scrollBar->FirstChild("name"));
								name = windowElement->GetText();

								windowElement = static_cast<TiXmlElement*>(scrollBar->FirstChild("skin"));
								skin = windowElement->GetText();
								
								windowElement = static_cast<TiXmlElement*>(scrollBar->FirstChild("position"));
								position = windowElement->GetText();

								windowElement = static_cast<TiXmlElement*>(scrollBar->FirstChild("size"));
								size = windowElement->GetText();

								windowElement = static_cast<TiXmlElement*>(scrollBar->FirstChild("track"));
								string track = windowElement->GetText();

								windowElement = static_cast<TiXmlElement*>(scrollBar->FirstChild("script"));

								MyGUI::ScrollBar *s = win->createWidget<MyGUI::ScrollBar>(skin, values[0], values[1], values[2], values[3], MyGUI::Align::Default, name);

								s->setScrollRange(atoi(size.c_str()));
								s->setTrackSize(atoi(track.c_str()));
								s->setScrollPosition(atoi(position.c_str()));
								s->eventMouseButtonPressed += newDelegate(this, &GUIManager::scrollBarMoved);

								if (windowElement){

									renderManager->logInfo("Adding script for GUI scroll bar...");
									string script = windowElement->GetText();
									scriptMap[s] = script;

								}

							} else {

								windowElement = static_cast<TiXmlElement*>(button->FirstChild("script"));

								if (windowElement){

									renderManager->logInfo("Adding script for GUI button...");
									string script = windowElement->GetText();
									scriptMap[b] = script;

								}

								b->eventMouseButtonPressed += newDelegate(this, &GUIManager::buttonPressed);

							}

						}

					} else {

						renderManager->logInfo("No buttons in this GUI.");

					}

					TiXmlNode *combosNode = window->FirstChild("combos");

					if (combosNode){

						for (TiXmlNode *comboNode = combosNode->FirstChild(); comboNode; comboNode = comboNode->NextSibling()){

							windowElement = static_cast<TiXmlElement*>(comboNode->FirstChild("name"));
							name = windowElement->GetText();

							windowElement = static_cast<TiXmlElement*>(comboNode->FirstChild("skin"));
							skin = windowElement->GetText();

							windowElement = static_cast<TiXmlElement*>(comboNode->FirstChild("position"));
							position = windowElement->GetText();

							windowElement = static_cast<TiXmlElement*>(comboNode->FirstChild("size"));
							size = windowElement->GetText();

							windowElement = static_cast<TiXmlElement*>(comboNode->FirstChild("font"));
							string font = windowElement->GetText();

							windowElement = static_cast<TiXmlElement*>(comboNode->FirstChild("selected"));
							string selected = windowElement->GetText();

							parseInts(position, values);
							parseInts(size, values + 2);
							parseInts(font, values + 4);

							MyGUI::ComboBox *c = win->createWidget<MyGUI::ComboBox>(skin, values[0], values[1], values[2], values[3], MyGUI::Align::Default, name);
							c->setFontHeight(values[4]);
							c->setTextColour(MyGUI::Colour(0,0,0));

							TiXmlNode *optionsNode = comboNode->FirstChild("options");

							if (optionsNode){

								for (TiXmlNode *optionNode = optionsNode->FirstChild(); optionNode; optionNode = optionNode->NextSibling()){

									string option = optionNode->ToElement()->GetText();
									c->addItem(option);

								}

							} else {

								renderManager->logWarn("This ComboBox has no options.");

							}

							c->setIndexSelected(atoi(selected.c_str()));

						}

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

