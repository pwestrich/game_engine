
#include "RenderManager.h"
#include "PhysicsManager.h"
#include "BulletMotionState.h"
#include "BulletDebugDrawer.h"
#include "BulletConvexHullCreator.h"

#include "btBulletDynamicsCommon.h"
#include "BulletMultiThreaded/btParallelConstraintSolver.h"
#include "BulletCollision/CollisionDispatch/btSphereSphereCollisionAlgorithm.h"
#include "BulletCollision/CollisionDispatch/btSphereTriangleCollisionAlgorithm.h"
#include "BulletCollision/CollisionDispatch/btSimulationIslandManager.h"
#include "BulletMultiThreaded/SpuGatheringCollisionDispatcher.h"
#include "BulletMultiThreaded/PlatformDefinitions.h"
#include "BulletMultiThreaded/Win32ThreadSupport.h"
#include "BulletMultiThreaded/SpuNarrowPhaseCollisionTask/SpuGatheringCollisionTask.h"

#define MAX_OUTSTANDING_TASKS	2

//turning on the debug lines seriously slows down the game
//I don't reccomend it
#define DEBUG_DRAW	0	//1 is on, 0 is off

using namespace std;

struct CustomMovementInfo {

	btRigidBody *body;

	btVector3 startGravity;
	btVector3 a0;
	btVector3 a1;
	
	float vx;
	float vy;
	float vz;

	float currentTime;
	float endTime;

};

PhysicsManager::PhysicsManager(RenderManager *rm){

	assert(rm != NULL);
	renderManager = rm;

	//create phyiscs stuff
	broadphaseInterface 	= new btDbvtBroadphase();

	//set up multithreaded stuff
	collisionThreadSupport 	= new Win32ThreadSupport(Win32ThreadSupport::Win32ThreadConstructionInfo(
									"collision", processCollisionTask,
									createCollisionLocalStoreMemory, MAX_OUTSTANDING_TASKS));
	solverThreadSupport 	= new Win32ThreadSupport(Win32ThreadSupport::Win32ThreadConstructionInfo(
									"solver", SolverThreadFunc, SolverlsMemoryFunc, MAX_OUTSTANDING_TASKS));

	//set up the collision detector
	btDefaultCollisionConstructionInfo cci;
	cci.m_defaultMaxPersistentManifoldPoolSize = 32768;
	collisionConfiguration	= new btDefaultCollisionConfiguration(cci);
	collisionDispatcher		= new SpuGatheringCollisionDispatcher(collisionThreadSupport, MAX_OUTSTANDING_TASKS ,collisionConfiguration);

	//constaint solver, whatever that is
	constraintSolver = new btParallelConstraintSolver(solverThreadSupport);

	//and the world itself
	world = new btDiscreteDynamicsWorld(collisionDispatcher, broadphaseInterface, constraintSolver, collisionConfiguration);
	world->getSimulationIslandManager()->setSplitIslands(false);
	world->getSolverInfo().m_numIterations = 4;
	world->getSolverInfo().m_solverMode = SOLVER_SIMD + SOLVER_USE_WARMSTARTING;//+SOLVER_RANDMIZE_ORDER;
	world->getDispatchInfo().m_enableSPU = true;

	//create debug grawer
	BulletDebugDrawer *debug = new BulletDebugDrawer(renderManager);
	debug->setDebugMode(DEBUG_DRAW);
	world->setDebugDrawer(debug);

}

PhysicsManager::~PhysicsManager(){

	//delete physics stuff in the opposite order they were created in
	delete world;
	delete constraintSolver;
	delete collisionDispatcher;
	delete collisionConfiguration;
	delete solverThreadSupport;
	delete collisionThreadSupport;
	delete broadphaseInterface;

}

void PhysicsManager::setGravity(const float x, const float y, const float z){

	world->setGravity(btVector3(x, y, z));

}

void PhysicsManager::applyTorque(const string &nodeName, const float x, const float y, const float z){

	try {

		btRigidBody *body = rigidBodies.at(nodeName);
		body->applyTorqueImpulse(btVector3(x, y, z));

	} catch (out_of_range &it){

		renderManager->logWarn("Attempt to apply torque to nonexistant node: " + nodeName);

	}

}

void PhysicsManager::applyForce(const string &nodeName, const float x, const float y, const float z){

	try {

		btRigidBody *body = rigidBodies.at(nodeName);
		body->applyCentralImpulse(btVector3(x, y, z));

	} catch (out_of_range &it){

		renderManager->logWarn("Attempt to apply force to nonexistant node: " + nodeName);

	}

}

void PhysicsManager::setLinearVelocity(const string &nodeName, const float x, const float y, const float z){

	try {

		btRigidBody *body = rigidBodies.at(nodeName);
		body->setLinearVelocity(btVector3(x, y, z));

	} catch (out_of_range &it){

		renderManager->logWarn("Attempt to set velocity of nonexistant node: " + nodeName);

	}

}

void PhysicsManager::setAngularVelocity(const string &nodeName, const float x, const float y, const float z){

	try {

		btRigidBody *body = rigidBodies.at(nodeName);
		body->setAngularVelocity(btVector3(x, y, z));

	} catch (out_of_range &it){

		renderManager->logWarn("Attempt to set angular velocity of nonexistant node: " + nodeName);

	}

}

void PhysicsManager::getLinearVelocity(const string &nodeName, float &x, float &y, float &z){

	try {

		btRigidBody *body = rigidBodies.at(nodeName);
		btVector3 velocity = body->getLinearVelocity();

		x = velocity.x();
		y = velocity.y();
		z = velocity.z();

	} catch (out_of_range &it){

		renderManager->logWarn("Attempt to get velocity of nonexistant node: " + nodeName);

	}

}

void PhysicsManager::getAngularVelocity(const string & nodeName, float &x, float &y, float &z){

	try {

		btRigidBody *body = rigidBodies.at(nodeName);
		btVector3 velocity = body->getAngularVelocity();

		x = velocity.x();
		y = velocity.y();
		z = velocity.z();

	} catch (out_of_range &it){

		renderManager->logWarn("Attempt to get velocity of nonexistant node: " + nodeName);

	}

}

//methods for custom object movement
void PhysicsManager::addCustomMovingObject(const string &objectName, const float time, const float ax0, const float ay0, const float az0, 
											const float ax1, const float ay1, const float az1){

	try {

		//attempt to find a body by name
		btRigidBody *body = rigidBodies.at(objectName);
		btVector3 velocity = body->getLinearVelocity();

		//create and set up information
		CustomMovementInfo *info = new CustomMovementInfo;

		info->body = body;
		btVector3 startGravity;
		info->a0 = btVector3(ax0, ay0, az0);
		info->a1 = btVector3(ax1, ay1, az1);
	
		info->vx = velocity.x();
		info->vy = velocity.y();
		info->vz = velocity.z();

		info->currentTime = 0.0;
		info->endTime = time;

		//save for later
		customMovement[objectName] = info;

	} catch (out_of_range &it){

		renderManager->logWarn("Attempt to create custom moving object for nonexistant object: " + objectName);

	}

}

void PhysicsManager::removeCustomMovingObject(const string &objectName){

	try {

		customMovement.at(objectName); 		//see if the object is actually there first
		customMovement.erase(objectName);	//then delete it

	} catch (out_of_range &it){

		renderManager->logWarn("Attempt to delete nonexistant custom movement info: " + objectName);

	}

}

//method to update physics every tick
void PhysicsManager::updatePhysics(const float timeStep){

	getWorldTransform();	//get any scene changes that may have happened since last tick
	tickCustomMovement(timeStep);
	world->stepSimulation(btScalar(timeStep), btScalar(10.0));
	world->debugDrawWorld();
	setWorldTransform(); 	//set the results of the physics calculations

}

//methods to set physics objects
void PhysicsManager::createRigidSphere(const string &nodeName, const float mass, const float r){

	btCollisionShape *shape = new btSphereShape(btScalar(r));
	createRigidBody(nodeName, shape, mass);

}

void PhysicsManager::createRigidBox(const string &nodeName, const float mass, const float x, const float y, const float z){

	btCollisionShape *shape = new btBoxShape(btVector3(x, y, z));
	createRigidBody(nodeName, shape, mass);

}

void PhysicsManager::createRigidCylinderX(const string &nodeName, const float mass, const float x, const float y, const float z){

	btCollisionShape *shape = new btCylinderShapeX(btVector3(x, y, z));
	createRigidBody(nodeName, shape, mass);

}

void PhysicsManager::createRigidHull(const string &nodeName, const float mass, BulletConvexHullCreator *hull){

	assert(hull != NULL);

	createRigidBody(nodeName, hull->getShape(), mass);

}

//private methods below here ----------------------------------------------------------------------
void PhysicsManager::createRigidBody(const string &nodeName, btCollisionShape *shape, const float mass){

	renderManager->logInfo("Creating rigid body: " + nodeName);

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
	body->activate(true);
	body->setSleepingThresholds(1.0, 1.0);

	//add it to the worls and our map
	rigidBodies[nodeName] = body;
	world->addRigidBody(body);

}

void PhysicsManager::getWorldTransform(){

	btAlignedObjectArray<btCollisionObject*> bodies = world->getCollisionObjectArray();

	for (int i = world->getNumCollisionObjects() - 1; i >= 0; --i){	

		btRigidBody *body = btRigidBody::upcast(bodies[i]);
		body->activate(true);
		BulletMotionState *motionState = static_cast<BulletMotionState*>(body->getMotionState());

		motionState->getCurrentTransform();

	}


}
void PhysicsManager::setWorldTransform(){

	btAlignedObjectArray<btCollisionObject*> bodies = world->getCollisionObjectArray();

	for (int i = world->getNumCollisionObjects() - 1; i >= 0; --i){	

		btRigidBody *body = btRigidBody::upcast(bodies[i]);
		body->activate(true);
		BulletMotionState *motionState = static_cast<BulletMotionState*>(body->getMotionState());

		btTransform currentTransform;
		motionState->getWorldTransform(currentTransform);
		body->setWorldTransform(currentTransform);

	}

}

void PhysicsManager::tickCustomMovement(const float timeStep){

	for (auto it = customMovement.begin(); it != customMovement.end(); ++it){

		//calculate accelleration at beginning of time interval
		float ax0 = it->second->a0.x() + it->second->startGravity.x() + it->second->a1.x() * it->second->currentTime;
		float ay0 = it->second->a0.y() + it->second->startGravity.y() + it->second->a1.y() * it->second->currentTime;
		float az0 = it->second->a0.z() + it->second->startGravity.z() + it->second->a1.z() * it->second->currentTime;

		//increm,ent time and check if done
		it->second->currentTime += timeStep;

		if (it->second->currentTime > it->second->endTime){

			it->second->body->setGravity(it->second->startGravity);
			customMovement.erase(it);
			continue;

		}

		//calculate ending accelleration for the frame
		float ax1 = ax0 + it->second->a1.x() * timeStep;
		float ay1 = ay0 + it->second->a1.y() * timeStep;
		float az1 = az0 + it->second->a1.z() * timeStep;

		//calculate average accelleration over time step
		float ax = (ax0 + ax1) / 2.0;
		float ay = (ay0 + ay1) / 2.0;
		float az = (az0 + az1) / 2.0;

		//integrate them
		it->second->vx += ax * timeStep;
		it->second->vy += ay * timeStep;
		it->second->vz += az * timeStep;

		//set valocity
		it->second->body->setLinearVelocity(btVector3(it->second->vx, it->second->vy, it->second->vz));

	}

}
