#ifndef __MOTION_STATE_H_INCLUDED__
#define __MOTION_STATE_H_INCLUDED__

#include <iostream>
#include "common.h"
#include "collisionobject.h"


/// This should be left for internal use by irrBullet.
/*!
    A motion state controls how to handle the visual representation of Bullet objects,
    and what to do on certain events such as setWorldTransform() and getWorldTransform().
*/
class IMotionState : public btDefaultMotionState
{
    public:
        IMotionState(const btTransform &initialPos);
        virtual ~IMotionState();


        virtual void setWorldTransform(const btTransform &worldTrans);

        void setObject(ICollisionObject* const obj) {object = obj;};

        virtual void getWorldTransform(btTransform &worldTrans) const;

    private:
        ICollisionObject *object;
        bool failed;

        btTransform worldTransform;
};

#endif // __MOTION_STATE_H_INCLUDED__

