
#include "RenderManager.h"
#include "PhysicsManager.h"

using namespace std;

PhysicsManager::PhysicsManager(RenderManager *rm){

	assert(rm != NULL);

	renderManager = rm;

}

PhysicsManager::~PhysicsManager(){}
