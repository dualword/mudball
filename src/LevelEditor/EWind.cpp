#include "EWind.h"

EWind::EWind(ISceneManager* smgr)
{
    this->smgr = smgr;

    mesh = smgr->getMesh("media/p_wind.b3d");
    node = smgr->addAnimatedMeshSceneNode((IAnimatedMesh*)mesh);
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
