
#ifndef GUI_MANAGER
#define GUI_MANAGER

#include <cassert>
#include <cstdlib>
#include <string>

#include "MyGUI.h"
#include "MyGUI_OgrePlatform.h"
#include "tinyxml.h"
#include "utilities.h"

using namespace std;

class RenderManager;

class GUIManager {

private:

	RenderManager *renderManager;
	MyGUI::Gui *gui;
	MyGUI::OgrePlatform *ogrePlatform;

	void buildGUIFromXML(const string &filename);

public:

	GUIManager(RenderManager *rm);
	~GUIManager();

	void loadResourceGroup(const string &groupName);
	void unloadResourceGroup();

};

#endif
