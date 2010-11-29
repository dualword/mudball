#include "ETubeBig.h"

ETubeBig::ETubeBig(ISceneManager* smgr)
{
    this->smgr = smgr;

    mesh = smgr->getMesh("media/p_tube_big.b3d");
    node = smgr->addMeshSceneNode((IAnimatedMesh*)mesh);

    type = ETUBEBIG;
}

void ETubeBig::setBulletPhysics(irrBulletWorld *world)
{
    this->world = world;
    shape = new IGImpactMeshShape(node, ((IMeshSceneNode*)node)->getMesh(),0);
    body = world->addRigidBody(shape);
}

void ETubeBig::updateBulletPhysics()
{
    matrix4 mat = node->getAbsoluteTransformation();
    body->setWorldTransform(mat);
}
