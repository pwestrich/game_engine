
#ifndef RENDER_LISTENER_H
#define RENDER_LISTENER_H

#include "Ogre.h"

using namespace std;

class RenderManager;

class RenderListener : public Ogre::FrameListener {

private:

	RenderManager *renderManager;
	bool render;

public:

	RenderListener(RenderManager *rm);
	virtual ~RenderListener();

	bool getRenderStatus();
	void stopRendering();

	virtual bool frameStarted(const Ogre::FrameEvent &event);
	virtual bool frameRenderingQueued(const Ogre::FrameEvent &event);
	virtual bool frameEnded(const Ogre::FrameEvent &event);

};

#endif
