
#include <cmath>
#include <sstream>

#include "InputManager.h"
#include "GameManager.h"

using namespace std;

InputManager::InputManager(GameManager *gman){

	gameManager = gman;

	listeners.reserve(8);

	windowHandle = gameManager->getWindowHandle();
	/*windowWidth = gameManager->getWindowWidth();
	windowHeight = gameManager->getWindowHeight();*/

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
		ss << windowHandle;

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

void InputManager::removeListener(InputListener *listener){

	assert(listener != NULL);

	for (size_t i = 0; i < listeners.size(); ++i){

		if (listeners[i] == listener){

			listeners.erase(listeners.begin() + i);
			return;

		}

	}

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

	KeyboardKey key = keyMap(e);

	if (key == KB_INVALID){

		//don't bother if it was an invalid key
		return true;

	}

	//othersise, notify the listeners
	for (size_t i = 0; i < listeners.size(); ++i){

		listeners[i]->keyPressed(key);

	}

	return true;

}

bool InputManager::keyReleased(const OIS::KeyEvent& e){

	for (size_t i = 0; i < listeners.size(); ++i){

		listeners[i]->keyReleased(keyMap(e));

	}

	return true; 

}

//mouse listener methods
bool InputManager::mouseMoved(const OIS::MouseEvent& e){

	//set the window width and height in the event
	e.state.width = gameManager->getWindowWidth(); //windowWidth;
	e.state.height = gameManager->getWindowHeight(); //windowHeight;

	//determine mouse info
	uint32_t x = e.state.X.abs;
	uint32_t y = e.state.Y.abs;
	int32_t dx = e.state.X.rel;
	int32_t dy = e.state.Y.rel;

	//and notify each listener
	for (size_t i = 0; i < listeners.size(); ++i){

		listeners[i]->mouseMoved(x, y, dx, dy);

	}

	return true;
}

bool InputManager::mousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID id){ 

   //set the window width and height in the event
   e.state.width = gameManager->getWindowWidth(); //windowWidth;
   e.state.height = gameManager->getWindowHeight(); //windowHeight;

   //determine mouse info
   uint32_t x = e.state.X.abs;
   uint32_t y = e.state.Y.abs;
   MouseButton button = mouseMap(id);

   //and notify each listener
   for (size_t i = 0; i < listeners.size(); ++i){

      listeners[i]->mousePressed(x, y, button);

   }

   return true;

}

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

	} else if (kc == OIS::KC_SPACE) {

		key = KB_SPACE;

	} else if (kc == OIS::KC_W){

		key = KB_W;

	} else if (kc == OIS::KC_A){

		key = KB_A;

	} else if (kc == OIS::KC_S){

		key = KB_S;

	} else if (kc == OIS::KC_D){

		key = KB_D;

	} else if (kc == OIS::KC_LSHIFT){

		key = KB_LSHIFT;

	} else if (kc == OIS::KC_UP){

		key = KB_UP;

	} else if (kc == OIS::KC_DOWN){

		key = KB_DOWN;

	} else if (kc == OIS::KC_LEFT){

		key = KB_LEFT;

	} else if (kc == OIS::KC_RIGHT){

		key = KB_RIGHT;

	} else if (kc == OIS::KC_TAB){

		key = KB_TAB;

	}

	return key;

}

MouseButton InputManager::mouseMap(const OIS::MouseButtonID id){

   switch (id) {

      case 0: return M_LEFT;
      case 1: return M_RIGHT;
      case 2: return M_MIDDLE;
      case 3: return M_BACK;
      case 4: return M_SCROLL;
      default: return M_INVALID;
   }

}
