#include "EPlatform.h"

EPlatform::EPlatform(ISceneManager* smgr)
{
    this->smgr = smgr;

    mesh = smgr->getMesh("media/p_plano.b3d");
    node = smgr->addMeshSceneNode(mesh);

    type = EPLATFORM;
}
