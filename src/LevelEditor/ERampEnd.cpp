#include "ERampEnd.h"

ERampEnd::ERampEnd(ISceneManager* smgr)
{
    this->smgr = smgr;

    mesh = smgr->getMesh("media/p_ramp_end.b3d");
    node = smgr->addAnimatedMeshSceneNode((IAnimatedMesh*)mesh);

    type = ERAMPEND;
}
