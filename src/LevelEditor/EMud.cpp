#include "EMud.h"

EMud::EMud(ISceneManager* smgr)
{
    this->smgr = smgr;

    mesh = smgr->getMesh("media/p_mud.b3d");
    node = smgr->addAnimatedMeshSceneNode((IAnimatedMesh*)mesh);

    type = EMUD;
}

