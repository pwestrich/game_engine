
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

using namespace std;

//helper function to create the solver
inline btThreadSupportInterface* createSolverThreadSupport(int maxNumThreads){

	Win32ThreadSupport::Win32ThreadConstructionInfo threadConstructionInfo("solver",SolverThreadFunc,SolverlsMemoryFunc,maxNumThreads);
	Win32ThreadSupport* threadSupport = new Win32ThreadSupport(threadConstructionInfo);
	threadSupport->startSPU();

	return threadSupport;
}

PhysicsManager::PhysicsManager(RenderManager *rm){

	assert(rm != NULL);

	renderManager = rm;

	//create phyiscs stuff
	broadphaseInterface 	= new btDbvtBroadphase();

	//set up the multithreaded thing
	collisionThreadSupport 	= new Win32ThreadSupport(Win32ThreadSupport::Win32ThreadConstructionInfo(
									"collision",
									processCollisionTask,
									createCollisionLocalStoreMemory,
									MAX_OUTSTANDING_TASKS));
	solverThreadSupport 	= createSolverThreadSupport(MAX_OUTSTANDING_TASKS);

	btDefaultCollisionConstructionInfo cci;
	cci.m_defaultMaxPersistentManifoldPoolSize = 32768;
	collisionConfiguration	= new btDefaultCollisionConfiguration(cci);
	collisionDispatcher		= new SpuGatheringCollisionDispatcher(collisionThreadSupport, MAX_OUTSTANDING_TASKS ,collisionConfiguration);//btCollisionDispatcher(collisionConfiguration);

	constraintSolver = new btParallelConstraintSolver(solverThreadSupport); //btSequentialImpulseConstraintSolver();
	
	world = new btDiscreteDynamicsWorld(collisionDispatcher, broadphaseInterface, constraintSolver, collisionConfiguration);
	world->getSimulationIslandManager()->setSplitIslands(false);
	world->getSolverInfo().m_numIterations = 4;
	world->getSolverInfo().m_solverMode = SOLVER_SIMD+SOLVER_USE_WARMSTARTING;//+SOLVER_RANDMIZE_ORDER;
	world->getDispatchInfo().m_enableSPU = true;

	//create debug grawer
	BulletDebugDrawer *debug = new BulletDebugDrawer(renderManager);
	debug->setDebugMode(0); 	//1 is on, 0 is off
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
		body->applyTorque(btVector3(x, y, z));

	} catch (out_of_range &it){

		renderManager->logWarn("Attempt to apply force to nonexistant node: " + nodeName);

	}

}

void PhysicsManager::applyForce(const string &nodeName, const float x, const float y, const float z){

	try {

		renderManager->logInfo("Applying force to: " + nodeName);
		btRigidBody *body = rigidBodies.at(nodeName);
		body->applyCentralImpulse(btVector3(x, y, z));

	} catch (out_of_range &it){

		renderManager->logWarn("Attempt to apply force to nonexistant node: " + nodeName);

	}

}

void PhysicsManager::updatePhysics(const float timeStep){

	world->stepSimulation(btScalar(timeStep), btScalar(5.0));
	world->debugDrawWorld();
	updateRigidBodies();

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

	//add it to the worls and our map
	rigidBodies[nodeName] = body;
	world->addRigidBody(body);

}
