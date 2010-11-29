#include "ETubeH.h"

ETubeH::ETubeH(ISceneManager* smgr)
{
    this->smgr = smgr;

    mesh = smgr->getMesh("media/p_tube_h.b3d");
    node = smgr->addMeshSceneNode((IAnimatedMesh*)mesh);

    type = ETUBEH;
}

void ETubeH::setBulletPhysics(irrBulletWorld *world)
{
    this->world = world;
    shape = new IGImpactMeshShape(node, ((IMeshSceneNode*)node)->getMesh(),0);
    body = world->addRigidBody(shape);
}

void ETubeH::updateBulletPhysics()
{
    matrix4 mat = node->getAbsoluteTransformation();
    body->setWorldTransform(mat);
}
