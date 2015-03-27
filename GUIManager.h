
#ifndef GUI_MANAGER
#define GUI_MANAGER

#include <cassert>
#include <cstdlib>
#include <string>

#include "MyGUI.h"
#include "MyGUI_OgrePlatform.h"
#include "tinyxml.h"
#include "utilities.h"

#include "InputListener.h"

using namespace std;

class RenderManager;

class GUIManager : public InputListener {

private:

	RenderManager *renderManager;
	MyGUI::Gui *gui;
	MyGUI::OgrePlatform *ogrePlatform;

	void buildGUIFromXML(const string &filename);

public:

	GUIManager(RenderManager *rm);
	~GUIManager();

	void loadResourceGroup(const string &filename, const string &groupName);
	void unloadResourceGroup();

	//InputListener methods
	void keyPressed(const KeyboardKey key);
	void keyReleased(const KeyboardKey key);

	void mouseMoved(const uint32_t x, const uint32_t y, const int32_t dx, const int32_t dy);
	void mousePressed(const uint32_t x, const uint32_t y, const MouseButton button);
	void mouseReleased(const uint32_t x, const uint32_t y, const MouseButton button);

	void joystickAxisMoved(const JoystickAxis axis, const uint32_t amount);
	void joystickButtonPressed(const JoystickButton button);

};

#endif
