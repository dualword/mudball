#ifndef __TRIANGLE_MESH_SHAPE_H_INCLUDED__
#define __TRIANGLE_MESH_SHAPE_H_INCLUDED__

#include "collisionshape.h"
#include "BulletCollision/CollisionShapes/btTriangleMesh.h"
#include "common.h"

namespace irr
{
    namespace scene
    {
        class IMesh;
    }
}

/// The triangle mesh shape, as the name suggests, is made up of triangles extracted from a mesh.
/*!
    @see IGImpactMeshShape
    @see IBvhTriangleMeshShape
*/
class ITriangleMeshShape : public ICollisionShape
{
    public:
        ITriangleMeshShape();
        virtual ~ITriangleMeshShape();

    protected:
        virtual void createShape(irr::scene::IMesh *collMesh);

        /// @return The btTriangleMesh created by extracting the geometry from an Irrlicht IMesh.
        btTriangleMesh *getTriangleMesh(irr::scene::IMesh *mesh);
};

#endif // __TRIANGLE_MESH_SHAPE_H_INCLUDED__
