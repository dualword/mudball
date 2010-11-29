#include "collisionobjectaffectordelete.h"
#include "bulletworld.h"
#include "collisionobject.h"
#include <vector3d.h>

ICollisionObjectAffectorDelete::ICollisionObjectAffectorDelete(irr::u32 delay)
{
    EndTime = 0;
    TimeDelay = delay;
    HasFinished = false;
    FirstRun = true;
}

void ICollisionObjectAffectorDelete::affectObject(ICollisionObject* object, irr::u32 timeMS)
{
    if(FirstRun == true)
    {
        EndTime = timeMS + TimeDelay;
        FirstRun = false;
    }

    else
    if(timeMS >= EndTime)
    {
        HasFinished = true;

        object->getDynamicsWorld()->addToDeletionQueue(object);
    }
}

ICollisionObjectAffectorDelete::~ICollisionObjectAffectorDelete()
{
    //dtor
}
