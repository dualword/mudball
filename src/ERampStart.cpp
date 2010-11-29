#include "ERampStart.h"

ERampStart::ERampStart(ISceneManager* smgr)
{
    this->smgr = smgr;

    mesh = smgr->getMesh("media/p_ramp_start.b3d");
    node = smgr->addMeshSceneNode((IAnimatedMesh*)mesh);

    type = ERAMPSTART;
}

void ERampStart::setBulletPhysics(irrBulletWorld *world)
{
    this->world = world;
    shape = new IGImpactMeshShape(node, ((IMeshSceneNode*)node)->getMesh(),0);
    body = world->addRigidBody(shape);
}

void ERampStart::updateBulletPhysics()
{
    matrix4 mat = node->getAbsoluteTransformation();
    body->setWorldTransform(mat);
}
