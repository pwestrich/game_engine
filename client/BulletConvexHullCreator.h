
#ifndef BULLET_HULL_CREATOR
#define BULLET_HULL_CREATOR

using namespace std;

class btConvexHullShape;
class Ogre::Vector3;

class BulletConvexHullCreator {

private:

	btConvexHullShape *shape;

public:

	BulletConvexHullCreator(const Ogre::Vector3 *verticies, const size_t numVerticies);
	~BulletConvexHullCreator();

	btConvexHullShape *getShape();

};


#endif
