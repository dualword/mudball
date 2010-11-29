#include "EGoal.h"

#include "App.h"

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

EGoal::EGoal(ISceneManager* smgr)
{
    this->smgr = smgr;

    mesh = smgr->getMesh("media/p_goal.b3d");
    node = smgr->addMeshSceneNode(smgr->getMesh("media/p_plano.b3d"));

    blueNode=smgr->addAnimatedMeshSceneNode((IAnimatedMesh*)mesh,node);
    blueNode->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);

    type = EGOAL;
}


//rotate and bounce the blue goal
void EGoal::animate()
{
    float ry = (blueNode->getRotation().Y+1);
    if(ry>=360) ry=0;
    blueNode->setRotation(vector3df(blueNode->getRotation().X,ry,blueNode->getRotation().Z));
    blueNode->setPosition(vector3df(0,sin(blueNode->getRotation().Y/15)/5+0.2,0));

    vector3df ballPos = App::getInstance()->getBallPosition();

    if(ballPos.getDistanceFrom(blueNode->getAbsolutePosition()) < 1)
    {
        App::getInstance()->setWin();
    }
}

void EGoal::setBulletPhysics(irrBulletWorld *world)
{
    this->world = world;
    shape = new IGImpactMeshShape(node, ((IMeshSceneNode*)node)->getMesh(),0);
    body = world->addRigidBody(shape);
}

void EGoal::updateBulletPhysics()
{
    matrix4 mat = node->getAbsoluteTransformation();
    body->setWorldTransform(mat);
}
