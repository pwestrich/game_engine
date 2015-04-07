
#include "RenderManager.h"
#include "PhysicsManager.h"
#include "BulletMotionState.h"

using namespace std;

PhysicsManager::PhysicsManager(RenderManager *rm){

	assert(rm != NULL);

	renderManager = rm;

	//create phyiscs stuff
	broadphaseInterface 	= new btDbvtBroadphase();
	collisionConfiguration	= new btDefaultCollisionConfiguration();
	collisionDispatcher		= new btCollisionDispatcher(collisionConfiguration);
	constraintSolver		= new btSequentialImpulseConstraintSolver();
	world					= new btDiscreteDynamicsWorld(collisionDispatcher, broadphaseInterface, constraintSolver, collisionConfiguration);

}

PhysicsManager::~PhysicsManager(){

	//delete physics stuff in the opposite order they were created in
	delete world;
	delete constraintSolver;
	delete collisionDispatcher;
	delete collisionConfiguration;
	delete broadphaseInterface;

}

void PhysicsManager::setGravity(const float &x, const float &y, const float &z){

	world->setGravity(btVector3(x, y, z));

}

void PhysicsManager::applyTorque(const string &nodeName, const float &x, const float &y, const float &z){

	try {

		btRigidBody *body = rigidBodies.at(nodeName);
		body->applyTorque(btVector3(x, y, z));

	} catch (out_of_range &it){

		renderManager->logWarn("Attempt to apply force to nonexistant node: " + nodeName);

	}

}

void PhysicsManager::updatePhysics(const float timeStep){

	world->stepSimulation(btScalar(timeStep), btScalar(10.0));

}

void PhysicsManager::updateRigidBodies(){

	btAlignedObjectArray<btCollisionObject*> bodies = world->getCollisionObjectArray();

	for (int i = world->getNumCollisionObjects() - 1; i >= 0; --i){

		btRigidBody *body = btRigidBody::upcast(bodies[i]);
		BulletMotionState *motionState = static_cast<BulletMotionState*>(body->getMotionState());

		btTransform currentTransform;
		motionState->getCurrentTransform();
		motionState->getWorldTransform(currentTransform);
		body->setWorldTransform(currentTransform);

	}

}

void PhysicsManager::createRigidSphere(const string &nodeName, const float &mass, const float &r){

	btCollisionShape *shape = new btSphereShape(btScalar(r));
	createRigidBody(nodeName, shape, mass);

}

void PhysicsManager::createRigidBox(const string &nodeName, const float &mass, const float &x, const float &y, const float &z){

	btCollisionShape *shape = new btBoxShape(btVector3(x, y, z));
	createRigidBody(nodeName, shape, mass);

}

void PhysicsManager::createRigidCylinderX(const string &nodeName, const float &mass, const float &x, const float &y, const float &z){

	btCollisionShape *shape = new btCylinderShapeX(btVector3(x, y, z));
	createRigidBody(nodeName, shape, mass);

}

//private methods below here ----------------------------------------------------------------------
void PhysicsManager::createRigidBody(const string &nodeName, btCollisionShape *shape, const float &mass){

	//create the motion state
	BulletMotionState *motionState = new BulletMotionState(renderManager, nodeName);

	//calculate inertia
	btScalar _mass(mass);
	btVector3 inertia(0,0,0);
	shape->calculateLocalInertia(mass, inertia);
	
	//create the body and set a few properties
	btRigidBody::btRigidBodyConstructionInfo bodyInfo(_mass, motionState, shape, inertia);
	btRigidBody *body = new btRigidBody(bodyInfo);

	body->setDamping(0.5, 0.5);
	body->setActivationState(DISABLE_DEACTIVATION);

	//add it to the worls and our map
	rigidBodies[nodeName] = body;
	world->addRigidBody(body);

}
