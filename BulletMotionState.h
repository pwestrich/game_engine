
#ifndef MOTION_STATE
#define MOTION_STATE

#include <string>

#include "btBulletDynamicsCommon.h"

using namespace std;

class RenderManager;

class BulletMotionState : public btMotionState {

private:

	RenderManager *renderManager;
	string sceneNodeName;
	btTransform transform;

public:

	BulletMotionState(RenderManager *rm, const string &newNodeName);
	~BulletMotionState();

	//required by btMotionState
	void getWorldTransform(btTransform& worldTrans) const;
	void setWorldTransform(const btTransform& worldTrans);

	void getCurrentTransform();

};

#endif
