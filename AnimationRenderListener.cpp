
#include "AnimationRenderListener.h"
#include "RenderManager.h"

using namespace std;

AnimationRenderListener::AnimationRenderListener(RenderManager *rm) : RenderListener(rm){}

AnimationRenderListener::~AnimationRenderListener(){}

//called as a new frame begins
bool AnimationRenderListener::frameStarted(const Ogre::FrameEvent &event){

	float timeStep = event.timeSinceLastFrame;

	getRenderManager()->processAnimations(timeStep);

	return getRenderStatus();

}

//called after the back buffer is flipped and the scene is presented to the display
bool AnimationRenderListener::frameRenderingQueued(const Ogre::FrameEvent &event){}

//called after the scene has rendered but before the back buffer is drawn
bool AnimationRenderListener::frameEnded(const Ogre::FrameEvent &event){}
