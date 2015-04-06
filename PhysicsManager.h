
#ifndef PHYSICS_MANAGER
#define PHYSICS_MANAGER



using namespace std;

class RenderManager;

class PhysicsManager {

private:

	RenderManager *renderManager;

public:

	PhysicsManager(RenderManager *rm);
	~PhysicsManager();

};

#endif
