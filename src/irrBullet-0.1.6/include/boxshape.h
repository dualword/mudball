#ifndef __BOX_SHAPE_H_INCLUDED__
#define __BOX_SHAPE_H_INCLUDED__

#include "collisionshape.h"


class IBoxShape : public ICollisionShape
{
public:
    IBoxShape(irr::scene::ISceneNode *n, irr::f32 m, bool overrideMargin = false);

    virtual ~IBoxShape();

protected:
    // Functions
    virtual void createShape(bool overrideMargin);
};


#endif // __BOX_SHAPE_H_INCLUDED__

