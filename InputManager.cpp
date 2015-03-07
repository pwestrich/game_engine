
#include <sstream>

#include "InputManager.h"
#include "GameManager.h"

using namespace std;

InputManager::InputManager(GameManager *gman){

	gameManager = gman;

	listeners.reserve(8);
	listeners.push_back(gameManager);

	OISManager = NULL;
	keyboard = NULL;
	mouse = NULL;
	joystick = NULL;

	gameManager->logInfo("Detecting input devices...");

	//try to get OIS to do things
	try {

		//create a paramater list for OIS and fill it in
		OIS::ParamList initList;
		stringstream  ss;
		ss << gameManager->getWindowHandle();

		initList.insert(make_pair("WINDOW", ss.str()));
		OISManager = OIS::InputManager::createInputSystem(initList);

		//now check for input devices
		if (OISManager->getNumberOfDevices(OIS::OISKeyboard) > 0){

			keyboard = static_cast<OIS::Keyboard*>(OISManager->createInputObject(OIS::OISKeyboard, true));
			keyboard->setEventCallback(this);

		}

		if (OISManager->getNumberOfDevices(OIS::OISMouse) > 0){

			mouse = static_cast<OIS::Mouse*>(OISManager->createInputObject(OIS::OISMouse, true));
			mouse->setEventCallback(this);

		}

		if (OISManager->getNumberOfDevices(OIS::OISJoyStick) > 0){

			joystick = static_cast<OIS::JoyStick*>(OISManager->createInputObject(OIS::OISJoyStick, true));
			joystick->setEventCallback(this);

		}

	} catch (exception &it){

		gameManager->logFatal(it.what(), __LINE__, __FILE__);

	} catch (...){

		gameManager->logFatal("Error initializing InputManager.", __LINE__, __FILE__);

	}

}

InputManager::~InputManager(){

	if (OISManager){

		OISManager->destroyInputSystem(OISManager);

	}

}

void InputManager::addListener(InputListener *newListener){

	assert(newListener != NULL);

	listeners.push_back(newListener);

}

void InputManager::checkForInput(const float time_step){

	if (keyboard){

		keyboard->capture();

	}

	if (mouse){

		mouse->capture();

	}

	if (joystick){

		joystick->capture();

	}

}

//key listener methods
bool InputManager::keyPressed(const OIS::KeyEvent& e){

	for (size_t i = 0; i < listeners.size(); ++i){

		listeners[i]->keyPressed(keyMap(e));

	}

	return true;

}

bool InputManager::keyReleased(const OIS::KeyEvent& e){ return true; }

//mouse listener methods
bool InputManager::mouseMoved(const OIS::MouseEvent& e){ return true; }
bool InputManager::mousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID id){ return true; }
bool InputManager::mouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID id){ return true; }

//joystick listener methods
bool InputManager::buttonPressed (const OIS::JoyStickEvent &arg, int button){ return true; }
bool InputManager::buttonReleased (const OIS::JoyStickEvent &arg, int button){ return true; }
bool InputManager::axisMoved (const OIS::JoyStickEvent &arg, int axis){ return true; }

//not common, do nothing here
bool InputManager::sliderMoved (const OIS::JoyStickEvent &, int index){ return true; }
bool InputManager::povMoved (const OIS::JoyStickEvent &arg, int index){ return true; }
bool InputManager::vector3Moved (const OIS::JoyStickEvent &arg, int index){ return true; }

//private methods below here ----------------------------------------------------------------------
KeyboardKey InputManager::keyMap(const OIS::KeyEvent &event){

	KeyboardKey key = KB_INVALID;
	OIS::KeyCode kc = event.key;

	//map keys to our system
	if (kc == OIS::KC_ESCAPE){

		key = KB_ESC;

	}

	return key;

}
