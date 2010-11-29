#include "EWind.h"

#include "App.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


EWind::EWind(ISceneManager* smgr)
{
    this->smgr = smgr;

    mesh = smgr->getMesh("media/p_wind.b3d");
    node = smgr->addMeshSceneNode((IAnimatedMesh*)mesh);
    hNode = smgr->addAnimatedMeshSceneNode(smgr->getMesh("media/p_wind_h.b3d"),node);
    hNode->setPosition(vector3df(0,0.9,0));
    wNode = smgr->addAnimatedMeshSceneNode(smgr->getMesh("media/p_wind_w.b3d"),node);
    wNode->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);
    wNode->setMaterialFlag(EMF_LIGHTING,false);
    wNode->setPosition(vector3df(0,0.2,-0.3));

    type = EWIND;
}

void EWind::animate()
{
    float rot=hNode->getRotation().Z+5;
    if(rot>=360) rot = 0;
    hNode->setRotation(vector3df(hNode->getRotation().X,hNode->getRotation().Y,rot));
}

void EWind::setBulletPhysics(irrBulletWorld *world)
{
    this->world = world;
    shape = new IGImpactMeshShape(node, ((IMeshSceneNode*)node)->getMesh(),0);
    body = world->addRigidBody(shape);
}

void EWind::updateBulletPhysics()
{
    matrix4 mat = node->getAbsoluteTransformation();
    body->setWorldTransform(mat);

    vector3df ballPos = App::getInstance()->getBallPosition();

    float windForce = 10;

    if(ballPos.getDistanceFrom(wNode->getAbsolutePosition()) < 1)
    {
        if(node->getRotation().Y == 0)
            App::getInstance()->applyForceToBall(vector3df(0,0,-windForce));
        else if(node->getRotation().Y == 180)
            App::getInstance()->applyForceToBall(vector3df(0,0,windForce));
        else if(node->getRotation().Y == 90)
            App::getInstance()->applyForceToBall(vector3df(-windForce,0,0));
        else if(node->getRotation().Y == 270)
            App::getInstance()->applyForceToBall(vector3df(windForce,0,0));
    }
}

void EWind::addShadow(EffectHandler* effect)
{
    effect->addShadowToNode(hNode, EFT_4PCF,ESM_BOTH);
    hNode->setMaterialFlag(EMF_LIGHTING,false);
}
