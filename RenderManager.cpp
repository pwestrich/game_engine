
#include "RenderManager.h"
#include "GameManager.h"

#include <iostream>

using namespace std;
using namespace Ogre;

RenderManager::RenderManager(GameManager *gman){

	cout << "Making RenderManager..." << endl;

	if (gman == NULL){

		exit(1);

	}

	gameManager = gman;

	//start creating ogre stuff
	root = NULL;
	window = NULL;
	sceneManager = NULL;
	camera = NULL;
	viewport = NULL;

	try {

		root = OGRE_NEW Ogre::Root("","");
		root->loadPlugin("RenderSystem_GL");

		Ogre::RenderSystem *renderer = root->getRenderSystemByName("OpenGL Rendering Subsystem");

		if (!renderer){

			cerr << "Error: OpenGL not avalible." << endl;
			exit(EXIT_FAILURE);

		}

		root->setRenderSystem(renderer);
		
		//set some options and make the window
		renderer->setConfigOption("Full Screen", "No");
        renderer->setConfigOption("Video Mode", "1024 x 768 @ 32-bit colour");
        window = root->initialise(true, "CSC 4903: Game Engine Programming");
        window->getCustomAttribute("WINDOW", &windowHandle);

	} catch (Ogre::Exception &it){

		cerr << "Exception while creating RenderManager: " << it.what() << endl;
		exit(EXIT_FAILURE);

	}

}

RenderManager::~RenderManager(){} //do nothing for now
