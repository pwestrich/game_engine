
#ifndef INPUT_MANAGER
#define INPUT_MANAGER

#include <sstream>
#include <vector>

#include "OIS.h"
#include "InputListener.h"

using namespace std;

class GameManager;
class InputListener;

class InputManager : public OIS::KeyListener, public OIS::MouseListener, public OIS::JoyStickListener {

private:

	//reference back to the game manager
	GameManager *gameManager;

	//something to hold the listeners
	std::vector<InputListener*> listeners;

	//the OIS manager
	OIS::InputManager *OISManager;

	//OIS input devices
	OIS::Keyboard *keyboard;
	OIS::Mouse *mouse;
	OIS::JoyStick *joystick;

	//window information
	size_t windowHandle;
	size_t windowWidth;
	size_t windowHeight;

	//function to map keys
	KeyboardKey keyMap(const OIS::KeyEvent &event);

   //maps mouse clicks
   MouseButton mouseMap(const OIS::MouseButtonID id);

public:

	InputManager(GameManager *gman);
	virtual ~InputManager();

	void addListener(InputListener *newListener);
	void removeListener(InputListener *listener);
	void checkForInput(const float time_step);

   //key listener methods
   bool keyPressed(const OIS::KeyEvent& e);
   bool keyReleased(const OIS::KeyEvent& e);

   //mouse listener methods
   bool mouseMoved(const OIS::MouseEvent& e);
   bool mousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID id);
   bool mouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID id);

   //joystick listener methods
   bool buttonPressed (const OIS::JoyStickEvent &arg, int button);
   bool buttonReleased (const OIS::JoyStickEvent &arg, int button);
   bool axisMoved (const OIS::JoyStickEvent &arg, int axis);

   //not common
   bool sliderMoved (const OIS::JoyStickEvent &, int index);
   bool povMoved (const OIS::JoyStickEvent &arg, int index);
   bool vector3Moved (const OIS::JoyStickEvent &arg, int index);

};

#endif
