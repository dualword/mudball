//MudBall-mod https://github.com/dualword/mudball
#include "EStart.h"

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

EStart::EStart(ISceneManager* smgr)
{
    this->smgr = smgr;

    mesh = smgr->getMesh("media/p_plano.b3d");
    node = smgr->addMeshSceneNode(mesh);

    type = ERAMPSTART;
}

void EStart::setBulletPhysics(irrBulletWorld *world)
{
    shape = new IGImpactMeshShape(node, ((IMeshSceneNode*)node)->getMesh(),0);
    body = world->addRigidBody(shape);
}

void EStart::updateBulletPhysics()
{
    matrix4 mat = node->getAbsoluteTransformation();
    body->setWorldTransform(mat);
}
