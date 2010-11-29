#include "ERampFlat.h"

ERampFlat::ERampFlat(ISceneManager* smgr)
{
    this->smgr = smgr;

    mesh = smgr->getMesh("media/p_ramp_flat.b3d");
    node = smgr->addMeshSceneNode((IAnimatedMesh*)mesh);

    type = ERAMPFLAT;
}

void ERampFlat::setBulletPhysics(irrBulletWorld *world)
{
    this->world = world;
    shape = new IGImpactMeshShape(node, ((IMeshSceneNode*)node)->getMesh(),0);
    body = world->addRigidBody(shape);
}

void ERampFlat::updateBulletPhysics()
{
    matrix4 mat = node->getAbsoluteTransformation();
    body->setWorldTransform(mat);
}
