#include "ETube2.h"

ETube2::ETube2(ISceneManager* smgr)
{
    this->smgr = smgr;

    mesh = smgr->getMesh("media/p_tube_2.b3d");
    node = smgr->addMeshSceneNode((IAnimatedMesh*)mesh);

    type = ETUBE2;
}

void ETube2::setBulletPhysics(irrBulletWorld *world)
{
    this->world = world;
    shape = new IGImpactMeshShape(node, ((IMeshSceneNode*)node)->getMesh(),0);
    body = world->addRigidBody(shape);
}

void ETube2::updateBulletPhysics()
{
    matrix4 mat = node->getAbsoluteTransformation();
    body->setWorldTransform(mat);
}
