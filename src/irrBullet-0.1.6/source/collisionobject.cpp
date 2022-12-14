//MudBall-mod https://github.com/dualword/mudball
#include "Bullet/btBulletDynamicsCommon.h"
#include "Bullet/btBulletCollisionCommon.h"
#include <ISceneManager.h>
#include <IFileSystem.h>
#include "bulletworld.h"
#include "collisionobject.h"
//#include "irrbullet_compile_config.h"

using namespace irr;
using namespace core;

ICollisionObject::ICollisionObject()
{
    static u32 collisionObjectID;
    collisionObjectID++;

    uniqueID = collisionObjectID;

    attributes = 0;

    IncludeNodeOnRemoval = true;
}

void ICollisionObject::updateObject()
{
    /*list<ICollisionObjectAffector*>::ConstIterator it = animators.begin();

    for(; it != animators.end(); it++)
    {
        (*it)->onAnimate();
    }*/
}

void ICollisionObject::setCollisionShape(ICollisionShape* const Shape)
{
    shape = Shape;
}


void ICollisionObject::setCcdValues(const f32 motionThreshold, const f32 radius)
{
    getPointer()->setCcdMotionThreshold(motionThreshold);
    getPointer()->setCcdSweptSphereRadius(radius);
}

void ICollisionObject::applyCalculatedCcdValues()
{
    const vector3df scale = shape->getSceneNode()->getScale();
    f32 f = 0.0;

    if(scale.X > f)
        f = scale.X;
    else if(scale.Y > f)
        f = scale.Y;
    else if(scale.Z > f)
        f = scale.Z;
    getPointer()->setCcdMotionThreshold(0.8);
	getPointer()->setCcdSweptSphereRadius(f * 0.5);
}


void ICollisionObject::setAnisotropicFriction(const vector3df &friction)
{
    getPointer()->setAnisotropicFriction(btVector3(friction.X,friction.Y,friction.Z));
}


void ICollisionObject::setContactProcessingThreshold(f32 cpt)
{
    getPointer()->setContactProcessingThreshold(cpt);
}


void ICollisionObject::setActivationState(EActivationState newState)
{
    getPointer()->setActivationState(newState);
}


void ICollisionObject::forceActivationState(EActivationState newState)
{
    getPointer()->forceActivationState(newState);
}


void ICollisionObject::activate(bool forceActivation)
{
    getPointer()->activate(forceActivation);
}


void ICollisionObject::setRestitution(f32 rest)
{
    getPointer()->setRestitution(rest);
}


void ICollisionObject::setInterpolationLinearVelocity(const vector3df &linvel)
{
    getPointer()->setInterpolationLinearVelocity(btVector3(linvel.X,linvel.Y,linvel.Z));
}


void ICollisionObject::setFriction(f32 friction)
{
    getPointer()->setFriction(friction);
}


void ICollisionObject::setInterpolationAngularVelocity(const vector3df &angvel)
{
    getPointer()->setInterpolationAngularVelocity(btVector3(angvel.X,angvel.Y,angvel.Z));
}


void ICollisionObject::setIslandTag(u32 tag)
{
    getPointer()->setIslandTag(tag);
}


void ICollisionObject::setCompanionId(u32 id)
{
    getPointer()->setCompanionId(id);
}


void ICollisionObject::setHitFraction(f32 hitFraction)
{
    getPointer()->setHitFraction(hitFraction);
}


void ICollisionObject::setCollisionFlags(ECollisionFlag flags)
{
    getPointer()->setCollisionFlags(flags);
}

void ICollisionObject::addAnimator(ICollisionObjectAffector* const anim)
{
    animators.push_back(anim);
}

void ICollisionObject::removeAnimator(ICollisionObjectAffector* const anim)
{
    // Remove the collision object
    if(anim)
    {
        list<ICollisionObjectAffector*>::Iterator it = animators.begin();

        for(; it != animators.end(); )
        {
            if((*it) == anim)
            {
                #ifdef IRRBULLET_DEBUG_MODE
                    printf("irrBullet: Removing animator (BODY: %i)\n", this->getUniqueID());
                #endif
                delete (*it);
                (*it) = 0;

                it = animators.erase(it);
            }

            else
                it++;
        }
    }
}

void ICollisionObject::removeAnimators()
{
    list<ICollisionObjectAffector*>::Iterator it = animators.begin();

    for(; it != animators.end(); )
    {
        #ifdef IRRBULLET_DEBUG_MODE
            printf("irrBullet: Removing animator (BODY: %i)\n", this->getUniqueID());
        #endif
        delete (*it);
        (*it) = 0;

        it = animators.erase(it);
    }
}



btCollisionObject *ICollisionObject::getPointer() const
{
    return object;
}

ICollisionShape *ICollisionObject::getCollisionShape() const
{
    return shape;
}


bool ICollisionObject::mergesSimulationIslands() const
{
    return getPointer()->mergesSimulationIslands();
}


vector3df ICollisionObject::getAnisotropicFriction() const
{
    const btVector3 friction = getPointer()->getAnisotropicFriction();

    return vector3df(friction.getX(), friction.getY(), friction.getZ());
}


bool ICollisionObject::hasAnisotropicFriction() const
{
    return getPointer()->hasAnisotropicFriction();
}


f32 ICollisionObject::getContactProcessingThreshold() const
{
    return f32(getPointer()->getContactProcessingThreshold());
}


bool ICollisionObject::hasContactResponse() const
{
    return getPointer()->hasContactResponse();
}


EActivationState ICollisionObject::getActivationState() const
{
    EActivationState state = EAS_SLEEPING;
    switch(getPointer()->getActivationState())
    {
        case 1:
            state = EAS_ACTIVE;
            break;
        case 2:
            state = EAS_SLEEPING;
            break;
        case 3:
            state = EAS_WANTS_DEACTIVATION;
            break;
        case 4:
            state = EAS_DISABLE_DEACTIVATION;
            break;
        case 5:
            state = EAS_DISABLE_SIMULATION;
    }

    return state;
}


bool ICollisionObject::isActive() const
{
    return getPointer()->isActive();
}


irr::f32 ICollisionObject::getRestitution() const
{
    return f32(getPointer()->getRestitution());
}


irr::f32 ICollisionObject::getFriction() const
{
    return f32(getPointer()->getFriction());
}


vector3df ICollisionObject::getInterpolationLinearVelocity() const
{
    const btVector3 vel = getPointer()->getInterpolationLinearVelocity();

    return vector3df(vel.getX(),vel.getY(),vel.getZ());
}


vector3df ICollisionObject::getInterpolationAngularVelocity() const
{
    const btVector3 vel = getPointer()->getInterpolationAngularVelocity();

    return vector3df(vel.getX(),vel.getY(),vel.getZ());
}


u32 ICollisionObject::getIslandTag() const
{
    return u32(getPointer()->getIslandTag());
}


u32 ICollisionObject::getCompanionId() const
{
    return u32(getPointer()->getCompanionId());
}


f32 ICollisionObject::getHitFraction() const
{
    return f32(getPointer()->getHitFraction());
}


ECollisionFlag ICollisionObject::getCollisionFlags() const
{
    return ECollisionFlag(getPointer()->getCollisionFlags());
}

bool ICollisionObject::checkCollideWith(ICollisionObject* const collObj) const
{
    return getPointer()->checkCollideWith(collObj->getPointer());
}

irr::u32 ICollisionObject::getNumAnimators() const
{
    u32 num = 0;

    list<ICollisionObjectAffector*>::ConstIterator it = animators.begin();

    for(; it != animators.end(); it++)
    {
        num++;
    }

    return num;
}

ICollisionObjectAffector* ICollisionObject::getAnimator(irr::u32 index)
{
    list<ICollisionObjectAffector*>::ConstIterator it = animators.begin();

    it += index;
    ICollisionObjectAffector *anim = (*it);
    if(anim)
        return anim;
    return 0;
}

bool ICollisionObject::hasCollidedWithAttribute(irr::c8 *attributeName) const
{
    for(irr::u32 i=0; i < dynamicsWorld->getNumManifolds(); i++)
    {
        btPersistentManifold* manifold = dynamicsWorld->getPointer()->getDispatcher()->getManifoldByIndexInternal(i);
        void* pointer = static_cast<btCollisionObject*>((manifold->getBody0())->getUserPointer());

        bool whichOne = (getUniqueID() == static_cast<SCollisionObjectIdentification*>(pointer)->ID);

//        ICollisionObject* other = (!whichOne) ?
//        dynamicsWorld->getCollisionObjectByID(
//            static_cast<SCollisionObjectIdentification*>(static_cast<btCollisionObject*>(manifold->getBody0())->getUserPointer())->ID) :
//        dynamicsWorld->getCollisionObjectByID(
//            static_cast<SCollisionObjectIdentification*>(static_cast<btCollisionObject*>(manifold->getBody1())->getUserPointer())->ID);
//
//
//
//        if(other->getAttributes()->existsAttribute(attributeName))
//            return other;
    }

    return false;
}

ICollisionObject::~ICollisionObject()
{
    #ifdef IRRBULLET_DEBUG_MODE
        switch(this->getObjectType())
        {
            case ECOT_RIGID_BODY:
                printf("irrBullet: Deleting rigid body (%i)\n", this->getUniqueID());
                break;
            case ECOT_SOFT_BODY:
                printf("irrBullet: Deleting soft body (%i)\n", this->getUniqueID());
                break;
            default:
                break;
        }
    #endif
    removeAnimators();

    if(IncludeNodeOnRemoval && shape->getSceneNode())
        shape->getSceneNode()->remove();

    if(shape)
        delete shape;

    if(collID)
        delete collID;

    if(object)
        delete object;

    if(attributes)
        attributes->drop();
}
