#include "ERampEnd.h"

ERampEnd::ERampEnd(ISceneManager* smgr)
{
    this->smgr = smgr;

    mesh = smgr->getMesh("media/p_ramp_end.b3d");
    node = smgr->addMeshSceneNode((IAnimatedMesh*)mesh);

    type = ERAMPEND;
}

void ERampEnd::setBulletPhysics(irrBulletWorld *world)
{
    this->world = world;
    shape = new IGImpactMeshShape(node, ((IMeshSceneNode*)node)->getMesh(),0);
    body = world->addRigidBody(shape);
}

void ERampEnd::updateBulletPhysics()
{
    matrix4 mat = node->getAbsoluteTransformation();
    body->setWorldTransform(mat);
}
