#include "ETubeBigCurve.h"

ETubeBigCurve::ETubeBigCurve(ISceneManager* smgr)
{
    this->smgr = smgr;

    mesh = smgr->getMesh("media/p_tube_big_curve.b3d");
    node = smgr->addAnimatedMeshSceneNode((IAnimatedMesh*)mesh);

    type = ETUBEBIGCURVE;
}
