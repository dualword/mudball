#include <ISceneNode.h>
#include "Bullet/btBulletDynamicsCommon.h"
#include "Bullet/btBulletCollisionCommon.h"
#include "boxshape.h"

using namespace irr;
using namespace core;
using namespace scene;

IBoxShape::IBoxShape(ISceneNode *n, f32 m, bool overrideMargin)
{
    node = n;
    mass = m;

    type = ECST_BOX;

    createShape(overrideMargin);
}

void IBoxShape::createShape(bool overrideMargin)
{
    node->updateAbsolutePosition();

    const vector3df& extent = node->getBoundingBox().getExtent() + f32((overrideMargin) ? 0.04:0.0);

    if(node->getType() == ESNT_CUBE)
    {
        shape = new btBoxShape(irrlichtToBulletVector(extent));
    }

    else
    {
        shape = new btBoxShape(irrlichtToBulletVector(extent * 0.5));
    }

	setLocalScaling(node->getScale(), ESP_COLLISIONSHAPE);
	calculateLocalInertia(getMass(), vector3df(0.0f,0.0f,0.0f));
}

IBoxShape::~IBoxShape()
{
}
