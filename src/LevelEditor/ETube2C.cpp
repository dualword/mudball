#include "ETube2C.h"

ETube2C::ETube2C(ISceneManager* smgr)
{
    this->smgr = smgr;

    mesh = smgr->getMesh("media/p_tube_2C.b3d");
    node = smgr->addAnimatedMeshSceneNode((IAnimatedMesh*)mesh);

    type = ETUBE2C;
}
