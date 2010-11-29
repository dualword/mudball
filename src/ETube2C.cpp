#include "ETube2C.h"

ETube2C::ETube2C(ISceneManager* smgr)
{
    this->smgr = smgr;

    mesh = smgr->getMesh("media/p_tube_2C.b3d");
    node = smgr->addMeshSceneNode((IAnimatedMesh*)mesh);

    type = ETUBE2C;
}

void ETube2C::setBulletPhysics(irrBulletWorld *world)
{
    this->world = world;
    shape = new IGImpactMeshShape(node, ((IMeshSceneNode*)node)->getMesh(),0);
    body = world->addRigidBody(shape);
}

void ETube2C::updateBulletPhysics()
{
    matrix4 mat = node->getAbsoluteTransformation();
    body->setWorldTransform(mat);
}
