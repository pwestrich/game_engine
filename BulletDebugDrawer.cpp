
#include "BulletDebugDrawer.h"
#include "RenderManager.h"

BulletDebugDrawer::BulletDebugDrawer(RenderManager *rm){

	assert (rm != NULL);

	renderManager = rm;

	renderManager->createManualObject("BulletDebugObject", "BulletDebugMaterial");
}

BulletDebugDrawer::~BulletDebugDrawer(){}
		
void BulletDebugDrawer::drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color){

	if (count == NUM_LINE_SEGMENTS_DRAWN){

		clearLines();
		count = 0;

	}

	renderManager->drawLine("BulletDebugObject", "BulletDebugMaterial", from.x(), from.y(), from.z(), to.x(), to.y(), to.z(), 0, 1, 0);
	count++;

}

void BulletDebugDrawer::drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime, const btVector3 &color){}

void BulletDebugDrawer::draw3dText(const btVector3 &location, const char* textString){}

void BulletDebugDrawer::reportErrorWarning(const char *warningString){

	renderManager->logWarn(warningString);

}
		
//Set how to draw the debug infor
void BulletDebugDrawer::setDebugMode(int debug_mode){

	debugMode = debug_mode;

}
	
//Not absolutely necessary but implemented for convenience, tells the btDynamicsWorld how to draw the debug info
int BulletDebugDrawer::getDebugMode() const {

	return debugMode;

}
		
void BulletDebugDrawer::clearLines(){

	renderManager->clearManualObject("BulletDebugObject");

}
