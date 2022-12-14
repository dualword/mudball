#ifndef __COLLISION_OBJECT_AFFECTOR_H__
#define __COLLISION_OBJECT_AFFECTOR_H__

#include <irrTypes.h>

class ICollisionObject;


/*!
    The base class for all collision object affectors. Derive from this class to create your own affectors.
*/
class ICollisionObjectAffector
{
    public:
        ICollisionObjectAffector();
        virtual ~ICollisionObjectAffector();

        virtual void affectObject(ICollisionObject* object, irr::u32 timeMS) = 0;

		bool hasFinished() const { return HasFinished; };

    protected:
        bool HasFinished;
};

#endif // __COLLISION_OBJECT_AFFECTOR_H__
