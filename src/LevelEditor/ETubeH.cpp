#include "ETubeH.h"

ETubeH::ETubeH(ISceneManager* smgr)
{
    this->smgr = smgr;

    mesh = smgr->getMesh("media/p_tube_h.b3d");
    node = smgr->addAnimatedMeshSceneNode((IAnimatedMesh*)mesh);

    type = ETUBEH;
}
