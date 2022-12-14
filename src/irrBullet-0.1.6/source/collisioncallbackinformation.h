#ifndef __COLLISION_CALLBACK_INFORMATION_H__
#define __COLLISION_CALLBACK_INFORMATION_H__

#include <irrTypes.h>
#include <vector3d.h>
#include "common.h"
#include "Bullet/BulletCollision/NarrowPhaseCollision/btPersistentManifold.h"


class ICollisionObject;
class irrBulletWorld;

struct SManifoldPoint
{
    public:
        SManifoldPoint() {};
        SManifoldPoint(const btManifoldPoint& point)
        {
            bulletManifoldPoint = point;
        };

        void setInfo(const btManifoldPoint& newPoint) { bulletManifoldPoint = newPoint; };


        irr::f32 getDistance() const { return irr::f32(bulletManifoldPoint.getDistance()); };
        irr::u32 getLifeTime() const { return bulletManifoldPoint.getLifeTime(); };
        irr::core::vector3df getPositionWorldOnA() const { return bulletToIrrlichtVector(bulletManifoldPoint.getPositionWorldOnA()); };
        irr::core::vector3df getPositionWorldOnB() const { return bulletToIrrlichtVector(bulletManifoldPoint.getPositionWorldOnB()); };
        void setDistance(irr::f32 dist) { bulletManifoldPoint.setDistance(dist); };
        irr::f32 getAppliedImpulse() const { return bulletManifoldPoint.getAppliedImpulse(); };

    private:
        btManifoldPoint bulletManifoldPoint;
};


class ICollisionCallbackInformation
{
    public:
        ICollisionCallbackInformation(btPersistentManifold* const manifold, irrBulletWorld* world);

        /// Note: both of the getBody() functions return a collision object with a matching unique ID to the corresponding btCollisionObject
        ICollisionObject* getBody0() const;
        ICollisionObject* getBody1() const;

        SManifoldPoint& getContactPoint(irr::u32 index);


    private:
        btPersistentManifold* contactManifold;
        SManifoldPoint manifoldPoint;
        irrBulletWorld *dynamicsWorld;
};

#endif // __COLLISION_CALLBACK_INFORMATION_H__
