#include "Bullet/btBulletDynamicsCommon.h"
#include "Bullet/btBulletCollisionCommon.h"
#include "bvhtrianglemeshshape.h"

using namespace irr;
using namespace core;
using namespace scene;

IBvhTriangleMeshShape::IBvhTriangleMeshShape(ISceneNode *n, IMesh *collMesh, f32 m)
{
    node = n;
    mass = m;

    type = ECST_BVHTRIMESH;

    createShape(collMesh);
}


void IBvhTriangleMeshShape::createShape(IMesh *mesh)
{
    btBvhTriangleMeshShape* bvhShape =  new btBvhTriangleMeshShape(getTriangleMesh(mesh), false, true);
    shape = bvhShape;

    calculateLocalInertia(getMass(), vector3df(0.0f,0.0f,0.0f));
}

IBvhTriangleMeshShape::~IBvhTriangleMeshShape()
{
    //dtor
}
