#include "EElevator.h"

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

EElevator::EElevator(ISceneManager* smgr)
{
    this->smgr = smgr;

    mesh = smgr->getMesh("media/p_elevator_body.b3d");
    node = smgr->addMeshSceneNode(mesh);
    node->setScale(vector3df(1,0.5,1));

    platformMesh = smgr->getMesh("media/p_elevator_platform.b3d");
    platformNode = smgr->addMeshSceneNode(platformMesh,node);
    platformNode->setScale(vector3df(1,2,1));

    dir = UP;
    count = 0;

    type = EELEVATOR;
}

void EElevator::animate()
{
    if(count == 0)
    {
        if(dir == UP)
            platformNode->setPosition(platformNode->getPosition() + vector3df(0,0.02,0));
        else
            platformNode->setPosition(platformNode->getPosition() + vector3df(0,-0.02,0));

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

void EElevator::setBulletPhysics(irrBulletWorld *world)
{
    this->world = world;
    printf("bullet elevator\n");
    shape = new IGImpactMeshShape(platformNode, ((IMeshSceneNode*)platformNode)->getMesh(),0);
    body = world->addRigidBody(shape);
}

void EElevator::updateBulletPhysics()
{
    matrix4 mat = platformNode->getAbsoluteTransformation();
    body->setWorldTransform(mat);
}

void EElevator::resetElement()
{
    platformNode->setPosition(vector3df(0,0,0));
}

void EElevator::addShadow(EffectHandler* effect)
{
    effect->addShadowToNode(platformNode, EFT_4PCF,ESM_BOTH);
    platformNode->setMaterialFlag(EMF_LIGHTING,false);
}
