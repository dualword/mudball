//MudBall-mod https://github.com/dualword/mudball
#include <IrrlichtDevice.h>
#include <IGUIEnvironment.h>
#include <IGUIStaticText.h>
#include "Bullet/btBulletDynamicsCommon.h"
#include "Bullet/btBulletCollisionCommon.h"
#include "Bullet/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h"
#include "bulletworld.h"
#include "raycastvehicle.h"
//#include "irrbullet_compile_config.h"


using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

irrBulletWorld::irrBulletWorld(IrrlichtDevice* const Device, bool useGImpact, bool useDebugDrawer) : device(Device)
{
    collisionConfiguration = new btDefaultCollisionConfiguration();
	//collisionConfiguration->setConvexConvexMultipointIterations();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	pairCache = new btDbvtBroadphase();
	constraintSolver = new btSequentialImpulseConstraintSolver();

    world = new btSoftRigidDynamicsWorld(dispatcher, pairCache,
        constraintSolver, collisionConfiguration);

    isPaused = false;

    if(useGImpact == true)
    {
        gimpactEnabled = true;
        btGImpactCollisionAlgorithm::registerAlgorithm((btCollisionDispatcher*)dispatcher);
    }

    else
    {
        gimpactEnabled = false;
    }



    // TODO: Fix IPhysicsDebugDraw so that it doesn't draw contact points even when debugging is off
    debug = 0;

    if(useDebugDrawer)
    {
        debug = new IPhysicsDebugDraw(device);
        world->setDebugDrawer(debug);

        debugMat.Lighting = false;
    }

    // For displaying debugging properties
    propertyText = device->getGUIEnvironment()->addStaticText(L"",
            rect<s32>(10,10,120,240), false);

    printf("irrBullet %i.%i.%i\n", IRRBULLET_VER_MAJOR, IRRBULLET_VER_MINOR, IRRBULLET_VER_MICRO);
}


int irrBulletWorld::stepSimulation(f32 timeStep, u32 maxSubSteps, f32 fixedTimeStep)
{
    if(isPaused == false)
    {
        getPointer()->stepSimulation(timeStep, maxSubSteps, fixedTimeStep);
        updateCollisionObjects();
    }

    return 0;
}



void irrBulletWorld::updateCollisionObjects()
{
    list<ICollisionObject*>::Iterator cbit = collisionObjects.begin();

    for(; cbit != collisionObjects.end(); cbit++)
    {
        ICollisionObject* obj = (*cbit);

        if(obj)
        {
            for(u32 j=0; j < obj->getNumAnimators(); j++)
            {
                ICollisionObjectAffector* anim = obj->getAnimator(j);
                if(anim->hasFinished() == false)
                {
                    anim->affectObject(obj, device->getTimer()->getTime());
                }
            }
        }
    }

    list<ICollisionObject*>::Iterator dlit = deletionList.begin();

    for(; dlit != deletionList.end(); )
    {
        this->removeCollisionObject((*dlit));
        dlit = deletionList.erase(dlit);
    }
}


void irrBulletWorld::registerGImpactAlgorithm()
{
    if(gimpactEnabled == false)
    {
        gimpactEnabled = true;
        btGImpactCollisionAlgorithm::registerAlgorithm((btCollisionDispatcher*)getPointer()->getDispatcher());
    }
}


IRigidBody* irrBulletWorld::addRigidBody(ICollisionShape* shape)
{
    IRigidBody* b = new IRigidBody(this, shape);
    collisionObjects.push_back(b);
    getPointer()->addRigidBody(b->getPointer());

    return b;
}


IRigidBody* irrBulletWorld::addRigidBody(ICollisionShape *shape, s32 group, s32 mask)
{
    IRigidBody* b = new IRigidBody(this, shape);
    collisionObjects.push_back(b);
    getPointer()->addRigidBody(b->getPointer(), group, mask);
    return b;
}


void irrBulletWorld::addToDeletionQueue(ICollisionObject* obj)
{
    if(!obj)
        return;
    deletionList.push_back(obj);
}


IRaycastVehicle* irrBulletWorld::addRaycastVehicle(IRigidBody* body, vector3d<s32> coordSys)
{
    IRaycastVehicle *vehicle = new IRaycastVehicle(body, this, coordSys);

    raycastVehicles.push_back(vehicle);

    getPointer()->addVehicle(vehicle->getPointer());

    return vehicle;
}


// TODO: Fix this function or other functions that may lead to its problem.
// It does not erase all objects, and seems to not update the list properly after
// deleting the rigid body.
void irrBulletWorld::removeCollisionObject(ICollisionObject *obj, bool deleteObject)
{
    // Remove the collision object
    if(obj)
    {
        list<ICollisionObject*>::Iterator cbit = collisionObjects.begin();

        for(; cbit != collisionObjects.end(); )
        {
            if((*cbit) == obj)
            {
                if((*cbit)->getObjectType() == ECOT_RIGID_BODY)
                {
                    #ifdef IRRBULLET_DEBUG_MODE
                        printf("irrBullet: Removing rigid body (%i)\n", obj->getUniqueID());
                    #endif
                    getPointer()->removeRigidBody(static_cast<IRigidBody*>(obj)->getPointer());
                }

                if(deleteObject == true)
                {
                    delete (*cbit);
                    (*cbit) = 0;
                }
                cbit = collisionObjects.erase(cbit);

            }

            else
                cbit++;
        }
    }
}

void irrBulletWorld::clearForces()
{
    getPointer()->clearForces();
}


void irrBulletWorld::setDebugMode(u32 mode)
{
    if(debug != 0)
        debug->setDebugMode(mode);
}


void irrBulletWorld::debugDrawWorld(bool setDriverMaterial)
{
    if(debug != 0)
    {
        if(setDriverMaterial)
        {
            device->getVideoDriver()->setMaterial(debugMat);
            device->getVideoDriver()->setTransform(irr::video::ETS_WORLD, irr::core::matrix4());
        }
        getPointer()->debugDrawWorld();
    }
}

void irrBulletWorld::debugDrawProperties(bool b, SColor col)
{
    if(b == true)
    {
        const u32 numObjects = getNumCollisionObjects();
        u32 active = 0;
        u32 sleeping = 0;

        for(u32 i=0; i < numObjects; i++)
        {
            if(getCollisionObject(i)->getActivationState() == EAS_ACTIVE)
                active++;

            else
            if(getCollisionObject(i)->getActivationState() == EAS_SLEEPING)
                sleeping++;
        }

        // Shows percentage of active objects.
        const f32 diff = active - sleeping;
        const s32 perc = (diff / active) * 100;


        stringw str = "MPF: "; // Milliseconds Per Frame
        str += (1000 / (device->getVideoDriver()->getFPS()));
        str += "\nObjects: ";
        str += active + sleeping;
        str += "\nActive: ";
        str += active;
        str += "\nSleeping: ";
        str += sleeping;
        str += "\nPerc. Active: ";
        str += (perc > 0) ? (stringw(perc) + "%") : "0%";

        propertyText->setText(str.c_str());

        if(propertyText->getOverrideColor() != col)
            propertyText->setOverrideColor(col);
    }

    else
    if(b == false)
    {
        if(propertyText->getText() != L"")
        {
            propertyText->setText(L"");
        }
    }
}


void irrBulletWorld::synchronizeMotionStates()
{
    getPointer()->synchronizeMotionStates();
}

void irrBulletWorld::synchronizeSingleMotionState(IRigidBody *body)
{
    getPointer()->synchronizeSingleMotionState(body->getPointer());
}

void irrBulletWorld::setGravity(const vector3df &gravity)
{
    getPointer()->setGravity(btVector3(gravity.X, gravity.Y, gravity.Z));
}


u32 irrBulletWorld::getNumCollisionObjects()
{
    u32 num = 0;

    list<ICollisionObject*>::Iterator it = collisionObjects.begin();

    for(; it != collisionObjects.end(); it++)
    {
        num++;
    }

    return num;
}


/*u32 irrBulletWorld::getNumCollisionObjects(EActivationState state)
{
    u32 num = 0;

    list<ICollisionObjects*>::Iterator it = collisionObjects.begin();

    for(; it != collisionObjects.end(); it++)
    {
        const bool isActive = (*it)->getPointer()->isActive();
        switch(isActive)
        {
            case true:
            {
                if(state == EAS_ACTIVE)
                {
                    num++;
                }
                break;
            }

            case false:
            {
                if(state == EAS_SLEEPING)
                {
                    num++;
                }
                break;
            }

            default:
                break;
        }
    }

    return num;
}*/


ICollisionObject* irrBulletWorld::getCollisionObject(irr::u32 index) const
{
    list<ICollisionObject*>::ConstIterator it = collisionObjects.begin();

    it += index;
    ICollisionObject *obj = (*it);
    if(obj)
        return obj;
    return 0;
}

ICollisionObject* irrBulletWorld::getCollisionObjectByID(irr::u32 ID) const
{
    list<ICollisionObject*>::ConstIterator it = collisionObjects.begin();

    for(; it != collisionObjects.end(); it++)
    {
        ICollisionObject* obj = (*it);
        if(obj->getUniqueID() == ID)
            return obj;

    }
    return 0;
}

ICollisionObject* irrBulletWorld::getCollisionObjectByName(const irr::core::stringw& name) const
{
    list<ICollisionObject*>::ConstIterator it = collisionObjects.begin();

    for(; it != collisionObjects.end(); it++)
    {
        ICollisionObject* obj = (*it);
        if(obj->getName() == name)
            return obj;

    }
}


ICollisionCallbackInformation* irrBulletWorld::getCollisionCallback(irr::u32 index) const
{
    ICollisionCallbackInformation *callback = new ICollisionCallbackInformation(dispatcher->getManifoldByIndexInternal(index), (irrBulletWorld*)this);
    return callback;
}


bool irrBulletWorld::isGImpactEnabled() const
{
    return gimpactEnabled;
}


irrBulletWorld::~irrBulletWorld()
{
    printf("-- irrBullet: Freeing memory --\n");
    // remove the raycast vehicles
    list<IRaycastVehicle*>::Iterator rvit = raycastVehicles.begin();

    for(; rvit != raycastVehicles.end(); )
    {
        IRaycastVehicle *vehicle = (*rvit);
        if(vehicle)
        {
            delete vehicle;
            vehicle = 0;

            rvit = raycastVehicles.erase(rvit);
        }
    }

    // Remove the collision objects
    /*list<ICollisionObject*>::Iterator rbit = collisionObjects.begin();

    for(; rbit != collisionObjects.end(); )
    {
        if(*rbit)
        {
            delete (*rbit);
            (*rbit) = 0;

            rbit = collisionObjects.erase(rbit);
        }
    }*/
    for(int i=0; i < getNumCollisionObjects(); i++)
    {
        removeCollisionObject(getCollisionObject(i));
    }


    if(debug != 0)
        delete debug;


    if(world)
        delete world;

    delete constraintSolver;
    delete pairCache;
    delete dispatcher;
    delete collisionConfiguration;
    printf("-- irrBullet: Finished freeing memory --\n");
}
