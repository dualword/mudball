#include "Bullet/btBulletDynamicsCommon.h"
#include "Bullet/btBulletCollisionCommon.h"
#include "Bullet/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h"
#include "gimpactmeshshape.h"


using namespace irr;
using namespace core;
using namespace scene;

IGImpactMeshShape::IGImpactMeshShape(ISceneNode *n, IMesh *collMesh, f32 m)
{
    node = n;
    mass = m;

    type = ECST_GIMPACT;

    createShape(collMesh);
}


void IGImpactMeshShape::createShape(IMesh *mesh)
{
	gimpactShape = new btGImpactMeshShape(getTriangleMesh(mesh));
    shape = gimpactShape;

    updateBound();

    calculateLocalInertia(getMass(), vector3df(0.0f,0.0f,0.0f));
}

// For some reason, updateBound only works the first time.
// All times after the first have no effect at all. Must fix this.
void IGImpactMeshShape::updateBound()
{
    gimpactShape->updateBound();
}


IGImpactMeshShape::~IGImpactMeshShape()
{
}
