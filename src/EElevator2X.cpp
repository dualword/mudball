#include "EElevator2X.h"

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

EElevator2X::EElevator2X(ISceneManager* smgr)
{
    this->smgr = smgr;

    mesh = smgr->getMesh("media/p_elevator_body.b3d");
    node = smgr->addMeshSceneNode(mesh);

    platformMesh = smgr->getMesh("media/p_elevator_platform.b3d");
    platformNode = smgr->addMeshSceneNode(platformMesh,node);

    dir = UP;
    count = 0;

    type = EELEVATOR2X;
}

void EElevator2X::animate()
{
    if(count == 0)
    {
        if(dir == UP)
            platformNode->setPosition(platformNode->getPosition() + vector3df(0,0.01,0));
        else
            platformNode->setPosition(platformNode->getPosition() + vector3df(0,-0.01,0));

        if(platformNode->getPosition().Y >= 2 && dir == UP)
        {
            dir = DOWN;
            count = 60;
        }
        if(platformNode->getPosition().Y <= 0 && dir == DOWN)
        {
            dir = UP;
            count = 60;
        }
    }
    else
        count--;
}

void EElevator2X::setBulletPhysics(irrBulletWorld *world)
{
    this->world = world;
    printf("bullet elevator2x\n");
    shape = new IGImpactMeshShape(platformNode, ((IMeshSceneNode*)platformNode)->getMesh(),0);
    body = world->addRigidBody(shape);
}

void EElevator2X::updateBulletPhysics()
{
    matrix4 mat = platformNode->getAbsoluteTransformation();
    body->setWorldTransform(mat);
}

void EElevator2X::resetElement()
{
    platformNode->setPosition(vector3df(0,0,0));
}

void EElevator2X::addShadow(EffectHandler* effect)
{
    effect->addShadowToNode(platformNode, EFT_4PCF,ESM_BOTH);
    platformNode->setMaterialFlag(EMF_LIGHTING,false);
}
