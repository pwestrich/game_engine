
#include "RenderManager.h"
#include "BulletMotionState.h"

using namespace std;

BulletMotionState::BulletMotionState(RenderManager *rm, const string &newNodeName){

	assert(rm != NULL);

	renderManager = rm;
	sceneNodeName = newNodeName;

	getCurrentTransform();

}

BulletMotionState::~BulletMotionState(){}

//required by btMotionState
void BulletMotionState::getWorldTransform(btTransform &worldTrans) const {

	worldTrans = transform;

}

void BulletMotionState::setWorldTransform(const btTransform &worldTrans){

	btQuaternion rotation = transform.getRotation();
	btVector3 position = transform.getOrigin();

	renderManager->rotateNode(sceneNodeName, rotation.w(), rotation.x(), rotation.y(), rotation.z());
	renderManager->translateNode(sceneNodeName, position.x(), position.y(), position.z());

}

void BulletMotionState::getCurrentTransform(){

	//get the current transformation of the node
	float w, x, y, z;

	renderManager->getRotation(sceneNodeName, w, x, y, z);
	transform.setRotation(btQuaternion(w, x, y, z));

	renderManager->getTranslation(sceneNodeName, x, y, z);
	transform.setOrigin(btVector3(x, y, z));

}
