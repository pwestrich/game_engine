
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

RenderManager *RenderListener::getRenderManager(){

	return renderManager;

}

void RenderListener::stopRendering(){

	render = false;

}

bool RenderListener::frameStarted(const Ogre::FrameEvent &event){}
bool RenderListener::frameRenderingQueued(const Ogre::FrameEvent &event){}
bool RenderListener::frameEnded(const Ogre::FrameEvent &event){}
