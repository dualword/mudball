#include "ERamp.h"

ERamp::ERamp(ISceneManager* smgr)
{
    this->smgr = smgr;

    mesh = smgr->getMesh("media/p_ramp.b3d");
    node = smgr->addMeshSceneNode((IAnimatedMesh*)mesh);

    type = ERAMP;
}

void ERamp::setBulletPhysics(irrBulletWorld *world)
{
    this->world = world;
    printf("bullet ramp\n");
    shape = new IGImpactMeshShape(node, ((IMeshSceneNode*)node)->getMesh(),0);
    body = world->addRigidBody(shape);
}

void ERamp::updateBulletPhysics()
{
    matrix4 mat = node->getAbsoluteTransformation();
    body->setWorldTransform(mat);
}
