
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

	//update the game
	renderManager->checkForInput(timeStep);
	renderManager->processAnimations(timeStep);
	renderManager->updatePhysics(timeStep);
	renderManager->updateAudio(timeStep);

	//slow down the framerate
	if (timeStep < (1.0 / 60.0)){

		boost::this_thread::sleep_for(boost::chrono::nanoseconds(static_cast<long long int>(((1.0 / 60.0) - timeStep) * 1000000000.0)));

	}

	//return this so we keep rendering
	return render;

}

//called after the back buffer is flipped and the scene is presented to the display
bool RenderListener::frameRenderingQueued(const Ogre::FrameEvent &event){ return render; }

//called after the scene has rendered but before the back buffer is drawn
bool RenderListener::frameEnded(const Ogre::FrameEvent &event){ return render; }
