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

    ball = smgr->addSphereSceneNode(0.5,30,node);
    ball->setMaterialTexture(0,smgr->getVideoDriver()->getTexture("media/ball.jpg"));
    ball->setPosition(vector3df(0,1,0));

    type = ESTART;
}
