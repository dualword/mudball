#include "EMud.h"

EMud::EMud(ISceneManager* smgr)
{
    this->smgr = smgr;

    mesh = smgr->getMesh("media/p_mud.b3d");
    node = smgr->addMeshSceneNode((IAnimatedMesh*)mesh);

    type = EMUD;
}

void EMud::setBulletPhysics(irrBulletWorld *world)
{
    printf("bullet mud\n");
    this->world = world;
    shape = new IGImpactMeshShape(node, ((IMeshSceneNode*)node)->getMesh(),0);
    body = world->addRigidBody(shape);
}

void EMud::updateBulletPhysics()
{
    matrix4 mat = node->getAbsoluteTransformation();
    body->setWorldTransform(mat);
}
