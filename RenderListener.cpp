
#include "RenderListener.h"
#include "RenderManager.h"

RenderListener::RenderListener(RenderManager *rm){

	renderManager = rm;
	render = true;

}

RenderListener::~RenderListener(){

	renderManager = NULL;

}

bool RenderListener::getRenderStatus(){

	return render;

}

void RenderListener::stopRendering(){

	render = false;

}

//called as a new frame begins
bool RenderListener::frameStarted(const Ogre::FrameEvent &event){

	//calculate time since last frame
	float timeStep = event.timeSinceLastFrame;

	//check for input and advance any animations
	renderManager->setTimeSinceLastFrame(timeStep);
	renderManager->checkForInput(timeStep);
	renderManager->processAnimations(timeStep);
	renderManager->updateMovement(timeStep);

	//return this so we keep rendering
	return render;

}

//called after the back buffer is flipped and the scene is presented to the display
bool RenderListener::frameRenderingQueued(const Ogre::FrameEvent &event){ return render; }

//called after the scene has rendered but before the back buffer is drawn
bool RenderListener::frameEnded(const Ogre::FrameEvent &event){ return render; }
