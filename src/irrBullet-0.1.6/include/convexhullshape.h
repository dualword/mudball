#ifndef __CONVEX_HULL_SHAPE_H_INLUDED__
#define __CONVEX_HULL_SHAPE_H_INLUDED__

#include "common.h"

class ICollisionShape;

namespace irr
{
    namespace scene
    {
        class IMesh;
    }
}

/// Convex hull shapes are for objects that need fast simulation with approximated shapes.
/*!
    These could be used for objects such as dynamic game level props.
*/
class IConvexHullShape : public ICollisionShape
{
    public:
        IConvexHullShape(irr::scene::ISceneNode *n, irr::scene::IMesh *collMesh, irr::f32 m);

        virtual ~IConvexHullShape();


    private:
        void createShape(irr::scene::IMesh *collMesh);

        void getConvexHull(irr::scene::IMesh *collMesh, btConvexHullShape *hullShape);
};

#endif // __CONVEX_HULL_SHAPE_H_INLUDED__
