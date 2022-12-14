#ifndef __SPHERE_SHAPE_H_INCLUDED__
#define __SPHERE_SHAPE_H_INCLUDED__


#include "collisionshape.h"


class ISphereShape : public ICollisionShape
{
public:
    ISphereShape(irr::scene::ISceneNode *n, irr::f32 m, bool overrideMargin = false);

    virtual ~ISphereShape();

protected:
    // Functions
    virtual void createShape(bool overrideMargin);
};


#endif // __SPHERE_SHAPE_H_INCLUDED__


