//MudBall-mod https://github.com/dualword/mudball
#include "collisioncallbackinformation.h"
#include "Bullet/BulletCollision/CollisionDispatch/btCollisionObject.h"
#include "collisionobject.h"
#include "bulletworld.h"

ICollisionCallbackInformation::ICollisionCallbackInformation(btPersistentManifold* const manifold, irrBulletWorld* world)
{
    contactManifold = manifold;
    dynamicsWorld = world;
}

ICollisionObject* ICollisionCallbackInformation::getBody0() const
{
    void* pointer = static_cast<btCollisionObject*>((contactManifold->getBody0())->getUserPointer());

    for(int i=0; i < dynamicsWorld->getNumCollisionObjects(); i++)
    {
        ICollisionObject* obj = dynamicsWorld->getCollisionObject(i);
        if(obj->getUniqueID() == static_cast<SCollisionObjectIdentification*>(pointer)->ID)
            return obj;
    }
}

ICollisionObject* ICollisionCallbackInformation::getBody1() const
{
    void* pointer = static_cast<btCollisionObject*>((contactManifold->getBody1())->getUserPointer());

    for(int i=0; i < dynamicsWorld->getNumCollisionObjects(); i++)
    {
        ICollisionObject* obj = dynamicsWorld->getCollisionObject(i);
        if(obj->getUniqueID() == static_cast<SCollisionObjectIdentification*>(pointer)->ID)
            return obj;
    }
}


SManifoldPoint& ICollisionCallbackInformation::getContactPoint(irr::u32 index)
{
    btManifoldPoint &point = contactManifold->getContactPoint(index);
    manifoldPoint.setInfo(point);
    return manifoldPoint;
}
