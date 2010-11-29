#include "ETube2H.h"

ETube2H::ETube2H(ISceneManager* smgr)
{
    this->smgr = smgr;

    mesh = smgr->getMesh("media/p_tube_2H.b3d");
    node = smgr->addMeshSceneNode((IAnimatedMesh*)mesh);

    type = ETUBE2H;
}

void ETube2H::setBulletPhysics(irrBulletWorld *world)
{
    this->world = world;
    shape = new IGImpactMeshShape(node, ((IMeshSceneNode*)node)->getMesh(),0);
    body = world->addRigidBody(shape);
}

void ETube2H::updateBulletPhysics()
{
    matrix4 mat = node->getAbsoluteTransformation();
    body->setWorldTransform(mat);
}
