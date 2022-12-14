#ifndef __BVH_TRIANGLE_MESH_SHAPE_H_INCLUDED__
#define __BVH_TRIANGLE_MESH_SHAPE_H_INCLUDED__

#include "trianglemeshshape.h"


/// The BVH triangle mesh shape is useful for static triangle mesh shapes, such as game levels.
class IBvhTriangleMeshShape : public ITriangleMeshShape
{
    public:
        IBvhTriangleMeshShape(irr::scene::ISceneNode *n, irr::scene::IMesh *collMesh, irr::f32 m);

        virtual ~IBvhTriangleMeshShape();


    protected:
        void createShape(irr::scene::IMesh *mesh);
};

#endif // __BVH_TRIANGLE_MESH_SHAPE_H_INCLUDED__
