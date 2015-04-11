
#include "btBulletDynamicsCommon.h"
#include "Ogre.h"

#include "BulletConvexHullCreator.h"

BulletConvexHullCreator::BulletConvexHullCreator(const Ogre::Vector3 *verticies, const size_t numVerticies){

	assert(verticies != NULL);
	assert(numVerticies > 3);

	shape = new btConvexHullShape();

	for (int i = 0; i < numVerticies; ++i){

		shape->addPoint(btVector3(verticies[i].x, verticies[i].y, verticies[i].z));

	}

}

BulletConvexHullCreator::~BulletConvexHullCreator(){

	shape = NULL;

}

btConvexHullShape *BulletConvexHullCreator::getShape(){

	return shape;

}
