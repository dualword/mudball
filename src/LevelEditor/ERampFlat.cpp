#include "ERampFlat.h"

ERampFlat::ERampFlat(ISceneManager* smgr)
{
    this->smgr = smgr;

    mesh = smgr->getMesh("media/p_ramp_flat.b3d");
    node = smgr->addAnimatedMeshSceneNode((IAnimatedMesh*)mesh);

    type = ERAMPFLAT;
}
