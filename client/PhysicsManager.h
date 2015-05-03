
#ifndef PHYSICS_MANAGER
#define PHYSICS_MANAGER

#include <map>
#include <string>

using namespace std;

class RenderManager;
class BulletConvexHullCreator;
class btBroadphaseInterface;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;
class btThreadSupportInterface;
class btConstraintSolver;
class btOverlappingPairCache;
class btCollisionShape;
class btRigidBody;
struct btCollisionAlgorithmCreateFunc;

struct CustomMovementInfo;

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

	//stuff for the multithreadedness
	btThreadSupportInterface *collisionThreadSupport;
	btThreadSupportInterface *solverThreadSupport;
	btCollisionAlgorithmCreateFunc *collisionFunc;

	//map to pair rigid bodies and their IDs
	map<string, btRigidBody*> rigidBodies;
	map<string, CustomMovementInfo*> customMovement;

	void createRigidBody(const string &nodeName, btCollisionShape *shape, const float mass);
	void getWorldTransform();
	void setWorldTransform();
	void tickCustomMovement(const float timeStep);

public:

	PhysicsManager(RenderManager *rm);
	~PhysicsManager();

	//methods to update the world
	void updatePhysics(const float timeStep);

	//methods to change the world's parameters
	void setGravity(const float x, const float y, const float z);

	//methods to move an object
	void applyTorque(const string &nodeName, const float x, const float y, const float z);
	void applyForce(const string &nodeName, const float x, const float y, const float z);
	void setLinearVelocity(const string &nodeName, const float x, const float y, const float z);
	void setAngularVelocity(const string &nodeName, const float x, const float y, const float z);

	void getLinearVelocity(const string &nodeName, float &x, float &y, float &z);
	void getAngularVelocity(const string & nodeName, float &x, float &y, float &z);
	void getGravity(float &x, float &y, float &z);

	//methods for custom object movement
	void addCustomMovingObject(const string &objectName, const float time, const float ax0, const float ay0, 
								const float az0, const float ax1, const float ay1, const float az1);
	void removeCustomMovingObject(const string &objectName);

	//methods to create an object
	void createRigidSphere(const string &nodeName, const float mass, const float r);
	void createRigidBox(const string &nodeName, const float mass, const float x, const float y, const float z);
	void createRigidCylinderX(const string &nodeName, const float mass, const float x, const float y, const float z);
	void createRigidHull(const string &nodeName, const float mass, BulletConvexHullCreator *hull);

};

#endif
