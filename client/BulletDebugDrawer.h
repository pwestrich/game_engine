
#ifndef DEBUG_DRAWER
#define DEBUG_DRAWER

#include <string>

#include "LinearMath/btIDebugDraw.h"

using namespace std;

class RenderManager;
class btVector3;

class BulletDebugDrawer : public btIDebugDraw {

private:

	RenderManager *renderManager;

	int debugMode;
	int count;

public:

	BulletDebugDrawer(RenderManager *rm);
	~BulletDebugDrawer();
		
	void drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color);
	void drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime, const btVector3 &color);
	void draw3dText(const btVector3 &location, const char* textString);
	void reportErrorWarning(const char *warningString);
		
	//Set how to draw the debug infor
	void setDebugMode(int debug_mode);
		
	//Not absolutely necessary but implemented for convenience, tells the btDynamicsWorld how to draw the debug info
	int getDebugMode() const;
		
	void clearLines();

};

#endif
