
#ifndef PHYSICS_MANAGER
#define PHYSICS_MANAGER

#include <map>
#include <string>

#include "btBulletDynamicsCommon.h"

using namespace std;

class RenderManager;

class PhysicsManager {

private:

	//reference to the RenderManager to interface with the game
	RenderManager *renderManager;

	//physics stuff
	btBroadphaseInterface 				*broadphaseInterface;
	btDefaultCollisionConfiguration 	*collisionConfiguration;
	btCollisionDispatcher 				*collisionDispatcher;
	btSequentialImpulseConstraintSolver *constraintSolver;
	btDiscreteDynamicsWorld 			*world;

	//map to pair rigid bodies and their IDs
	map<string, btRigidBody*> rigidBodies;

	void createRigidBody(const string &nodeName, btCollisionShape *shape, const float &mass);

public:

	PhysicsManager(RenderManager *rm);
	~PhysicsManager();

	//methods to update the world
	void updatePhysics(const float timeStep);
	void updateRigidBodies();

	//methods to change the world's parameters
	void setGravity(const float &x, const float &y, const float &z);

	//methods to move an object
	void applyTorque(const string &nodeName, const float &x, const float &y, const float &z);

	//methods to create an object
	void createRigidSphere(const string &nodeName, const float &mass, const float &r);
	void createRigidBox(const string &nodeName, const float &mass, const float &x, const float &y, const float &z);
	void createRigidCylinderX(const string &nodeName, const float &mass, const float &x, const float &y, const float &z);

};

#endif
