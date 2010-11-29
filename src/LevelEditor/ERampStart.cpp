#include "ERampStart.h"

ERampStart::ERampStart(ISceneManager* smgr)
{
    this->smgr = smgr;

    mesh = smgr->getMesh("media/p_ramp_start.b3d");
    node = smgr->addAnimatedMeshSceneNode((IAnimatedMesh*)mesh);

    type = ERAMPSTART;
}
