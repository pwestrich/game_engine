
#ifndef ANIMATION_LISTENER_H
#define ANIMATION_LISTENER_H

#include "RenderListener.h"

class AnimationRenderListener : public RenderListener {

private:


public:

	AnimationRenderListener(RenderManager *rm);
	~AnimationRenderListener();

	//called as a new frame begins
    bool frameStarted(const Ogre::FrameEvent &event);

    //called after the back buffer is flipped and the scene is presented to the display
    bool frameRenderingQueued(const Ogre::FrameEvent &event);

    //called after the scene has rendered but before the back buffer is drawn
    bool frameEnded(const Ogre::FrameEvent &event);

};

#endif
