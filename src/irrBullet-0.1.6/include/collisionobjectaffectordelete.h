#ifndef __COLLISION_OBJECT_AFFECTOR_DELETE_H__
#define __COLLISION_OBJECT_AFFECTOR_DELETE_H__

#include "collisionobjectaffector.h"
#include <irrTypes.h>

class ICollisionObject;

/*!
    The delete affector adds a collision object to the deletion queue(and its corresponding scene node, if RemoveNode is true)
    from the dynamics world after the given time (TimeDelay) has elapsed.
*/
class ICollisionObjectAffectorDelete : public ICollisionObjectAffector
{
    public:
        /*!
            @param delay The amount of time to wait before removing the object
            @param remNode If this is true, the corresponding scene node will also be removed.
        */
        ICollisionObjectAffectorDelete(irr::u32 delay);
        virtual ~ICollisionObjectAffectorDelete();

        virtual void affectObject(ICollisionObject* object, irr::u32 timeMS);


    protected:
        irr::u32 EndTime;
        irr::u32 TimeDelay;
        bool FirstRun;
};

#endif // __COLLISION_OBJECT_AFFECTOR_DELETE_H__
