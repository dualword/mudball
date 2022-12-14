//MudBall-mod https://github.com/dualword/mudball
#include "Bullet/btBulletDynamicsCommon.h"
#include "Bullet/btBulletCollisionCommon.h"
#include <ISceneManager.h>
#include <IFileSystem.h>
#include "rigidbody.h"
#include "collisionshape.h"
#include "motionstate.h"
//#include "irrbullet_compile_config.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

IRigidBody::IRigidBody(irrBulletWorld* const world, ICollisionShape *collShape)
{
    #ifdef IRRBULLET_DEBUG_MODE
        printf("irrBullet: Creating rigid body (%i)\n", this->getUniqueID());
    #endif
    shape = collShape;

    objectType = ECOT_RIGID_BODY;

    dynamicsWorld = world;


    const vector3df pos = shape->getSceneNode()->getPosition();

    irr::core::matrix4 irrmat;
    irrmat.setTranslation(shape->getSceneNode()->getPosition());
    irrmat.setRotationDegrees(shape->getSceneNode()->getRotation());


    // Set the initial position of the object
	btTransform transform;
    btTransformFromIrrlichtMatrix(irrmat, transform);
	//transform.setIdentity();
	//transform.setOrigin(irrlichtToBulletVector(pos));

	IMotionState *motionState = new IMotionState(transform);
	motionState->setObject(this);

    vector3df in = shape->getLocalInertia();


    object = new btRigidBody(shape->getMass(), motionState, shape->getPointer(),
        irrlichtToBulletVector(in));

    attributes = getCollisionShape()->getSceneNode()->getSceneManager()->getFileSystem()->createEmptyAttributes(
        getCollisionShape()->getSceneNode()->getSceneManager()->getVideoDriver());

    collID = new SCollisionObjectIdentification();
    collID->ID = getUniqueID();
    collID->name = "RigidBody";

    object->setUserPointer(collID);
}


IRigidBody::IRigidBody(irrBulletWorld* const world, SRigidBodyConstructionInfo info)
{
    shape = info.collisionShape;

    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(irrlichtToBulletVector(info.startVector));

    IMotionState *motionState = new IMotionState(transform);
    motionState->setObject(this);

    vector3df in = shape->getLocalInertia();
    object = new btRigidBody(shape->getMass(), motionState, shape->getPointer(),
        btVector3(in.X,in.Y,in.Z));

    getPointer()->setDamping(info.linearDamping, info.angularDamping);

    collID = new SCollisionObjectIdentification();
    collID->ID = getUniqueID();
    collID->name = "RigidBody";

    object->setUserPointer(collID);

}

void IRigidBody::translate(const vector3df &v)
{
    getPointer()->translate(irrlichtToBulletVector(v));
}

void IRigidBody::getAabb(vector3df &aabbMin, vector3df &aabbMax) const
{
    btVector3 aabbMinBt, aabbMaxBt;
    getPointer()->getAabb(aabbMinBt, aabbMaxBt);

    aabbMin = vector3df(aabbMinBt.getX(),aabbMinBt.getY(),aabbMinBt.getZ());
    aabbMax = vector3df(aabbMaxBt.getX(),aabbMaxBt.getY(),aabbMaxBt.getZ());
}


void IRigidBody::updateDeactivation(f32 timeStep)
{
    getPointer()->updateDeactivation(timeStep);
}


void IRigidBody::setLinearVelocity(const vector3df &linVel, ERBTransformSpace transformSpace)
{
    getPointer()->setLinearVelocity((transformSpace==ERBTS_WORLD) ?
        irrlichtToBulletVector(linVel) :
        btVector3(getPointer()->getWorldTransform().getBasis()*irrlichtToBulletVector(linVel)));
}


void IRigidBody::setAngularVelocity(const vector3df &angVel, ERBTransformSpace transformSpace)
{
    getPointer()->setAngularVelocity((transformSpace==ERBTS_WORLD) ?
        irrlichtToBulletVector(angVel) :
        btVector3(getPointer()->getWorldTransform().getBasis()*irrlichtToBulletVector(angVel)));
}


void IRigidBody::saveKinematicState(f32 step)
{
    getPointer()->saveKinematicState(step);
}


void IRigidBody::applyCentralForce(const vector3df &force, ERBTransformSpace transformSpace)
{
    getPointer()->applyCentralForce((transformSpace==ERBTS_WORLD) ?
        irrlichtToBulletVector(force) :
        btVector3(getPointer()->getWorldTransform().getBasis()*irrlichtToBulletVector(force)));
}


void IRigidBody::applyForce(const vector3df &force, const vector3df &relPos, ERBTransformSpace transformSpace)
{
    getPointer()->applyForce((transformSpace==ERBTS_WORLD) ?
        irrlichtToBulletVector(force) :
        btVector3(getPointer()->getWorldTransform().getBasis()*irrlichtToBulletVector(force)),
        irrlichtToBulletVector(relPos));
}


void IRigidBody::applyCentralImpulse(const vector3df &impulse, ERBTransformSpace transformSpace)
{
    getPointer()->applyCentralImpulse((transformSpace==ERBTS_WORLD) ?
        irrlichtToBulletVector(impulse) :
        btVector3(getPointer()->getWorldTransform().getBasis()*irrlichtToBulletVector(impulse)));
}


void IRigidBody::applyImpulse(const vector3df &impulse, const vector3df &relPos, ERBTransformSpace transformSpace)
{
    getPointer()->applyImpulse((transformSpace==ERBTS_WORLD) ?
        irrlichtToBulletVector(impulse) :
        btVector3(getPointer()->getWorldTransform().getBasis()*irrlichtToBulletVector(impulse)),
        irrlichtToBulletVector(relPos));
}


void IRigidBody::applyTorque(const vector3df &torque, ERBTransformSpace transformSpace)
{
    getPointer()->applyTorque((transformSpace==ERBTS_WORLD) ?
        irrlichtToBulletVector(torque) :
        btVector3(getPointer()->getWorldTransform().getBasis()*irrlichtToBulletVector(torque)));
}

void IRigidBody::applyTorqueImpulse(const vector3df &torque, ERBTransformSpace transformSpace)
{
    getPointer()->applyTorqueImpulse((transformSpace==ERBTS_WORLD) ?
        irrlichtToBulletVector(torque) :
        btVector3(getPointer()->getWorldTransform().getBasis()*irrlichtToBulletVector(torque)));
}


void IRigidBody::internalApplyImpulse(const vector3df &linearComponent,
    const vector3df &angularComponent, f32 impulseMagnitude, ERBTransformSpace linTransformSpace, ERBTransformSpace angTransformSpace)
{
//    getPointer()->internalApplyImpulse((linTransformSpace==ERBTS_WORLD) ? irrlichtToBulletVector(linearComponent) :
//        btVector3(getPointer()->getWorldTransform().getBasis()*irrlichtToBulletVector(linearComponent)),
//        (angTransformSpace==ERBTS_WORLD) ? irrlichtToBulletVector(angularComponent) :
//        btVector3(getPointer()->getWorldTransform().getBasis()*irrlichtToBulletVector(angularComponent)),
//        btScalar(impulseMagnitude));
}


void IRigidBody::clearForces()
{
    getPointer()->clearForces();
}


void IRigidBody::updateInertiaTensor()
{
    getPointer()->updateInertiaTensor();
}


void IRigidBody::setDamping(const f32 lin_damping, const f32 ang_damping)
{
    getPointer()->setDamping((btScalar)lin_damping, (btScalar)ang_damping);
}


void IRigidBody::applyGravity()
{
    getPointer()->applyGravity();
}


void IRigidBody::setGravity(const vector3df &grav)
{
    getPointer()->setGravity(irrlichtToBulletVector(grav));
}


void IRigidBody::applyDamping(f32 timeStep)
{
    getPointer()->applyDamping(timeStep);
}


void IRigidBody::setMassProps(f32 mass, const vector3df &inertia)
{
    getPointer()->setMassProps(mass, irrlichtToBulletVector(inertia));
}


void IRigidBody::setLinearFactor(const vector3df &linearFactor)
{
    getPointer()->setLinearFactor(irrlichtToBulletVector(linearFactor));
}


void IRigidBody::integrateVelocities(f32 step)
{
    getPointer()->integrateVelocities(step);
}


void IRigidBody::setInvInertiaDiagLocal(const vector3df &diagInvInertia)
{
    getPointer()->setInvInertiaDiagLocal(irrlichtToBulletVector(diagInvInertia));
}


void IRigidBody::setSleepingThresholds(f32 linear, f32 angular)
{
    getPointer()->setSleepingThresholds(linear, angular);
}


void IRigidBody::setWorldTransform(irr::core::matrix4 irrmat)
{
    btTransform transform;
    btTransformFromIrrlichtMatrix(irrmat, transform);

    getPointer()->setWorldTransform(transform);
}


void IRigidBody::setAngularFactor(const vector3df &angFac)
{
    getPointer()->setAngularFactor(irrlichtToBulletVector(angFac));
}


void IRigidBody::setAngularFactor(f32 angFac)
{
    getPointer()->setAngularFactor(angFac);
}




vector3df IRigidBody::getLinearVelocity() const
{
    const btVector3 linVec = getPointer()->getLinearVelocity();

    return bulletToIrrlichtVector(linVec);
}


vector3df IRigidBody::getAngularVelocity() const
{
    const btVector3 angVec = getPointer()->getAngularVelocity();

    return bulletToIrrlichtVector(angVec);
}


vector3df IRigidBody::getVelocityInLocalPoint(const vector3df &relPos) const
{
    const btVector3 locVel = getPointer()->getVelocityInLocalPoint(btVector3(relPos.X,relPos.Y,relPos.Z));

    return bulletToIrrlichtVector(locVel);
}


f32 IRigidBody::computeImpulseDenominator(const vector3df &pos, const vector3df &normal) const
{
    return f32(getPointer()->computeImpulseDenominator(
        btVector3(pos.X,pos.Y,pos.Z), btVector3(normal.X,normal.Y,normal.Z)));
}


f32 IRigidBody::computeAngularImpulseDenominator(const vector3df &axis) const
{
    return f32(getPointer()->computeAngularImpulseDenominator(btVector3(axis.X,axis.Y,axis.Z)));
}


bool IRigidBody::wantsSleeping()
{
    return getPointer()->wantsSleeping();
}


vector3df IRigidBody::getTotalForce() const
{
    const btVector3 force = getPointer()->getTotalForce();

    return bulletToIrrlichtVector(force);
}

vector3df IRigidBody::getTotalTorque()
{
    const btVector3 torque = getPointer()->getTotalTorque();

    return bulletToIrrlichtVector(torque);
}


vector3df IRigidBody::getGravity() const
{
    btVector3 grav = static_cast<btRigidBody*>(object)->getGravity();

    return bulletToIrrlichtVector(grav);
}


f32 IRigidBody::getLinearDamping() const
{
    return f32(getPointer()->getLinearDamping());
}


f32 IRigidBody::getAngularDamping() const
{
    return f32(getPointer()->getAngularDamping());
}


f32 IRigidBody::getLinearSleepingThreshold() const
{
    return f32(getPointer()->getLinearSleepingThreshold());
}


f32 IRigidBody::getAngularSleepingThreshold() const
{
    return f32(getPointer()->getAngularSleepingThreshold());
}


vector3df IRigidBody::getLinearFactor() const
{
    const btVector3 linearFactor = getPointer()->getLinearFactor();

    return bulletToIrrlichtVector(linearFactor);
}


f32 IRigidBody::getInvMass() const
{
    return f32(getPointer()->getInvMass());
}


vector3df IRigidBody::getInvInertiaDiagLocal() const
{
    const btVector3 diagInvInertia = getPointer()->getInvInertiaDiagLocal();

    return bulletToIrrlichtVector(diagInvInertia);
}


vector3df IRigidBody::getAngularFactor() const
{
    const btVector3 angFac = getPointer()->getAngularFactor();

    return bulletToIrrlichtVector(angFac);
}


bool IRigidBody::isInWorld() const
{
    return getPointer()->isInWorld();
}


bool IRigidBody::checkCollideWithOverride(ICollisionObject *co)
{
    return getPointer()->checkCollideWithOverride(co->getPointer());
}


u32 IRigidBody::getNumConstraintRefs() const
{
    return u32(getPointer()->getNumConstraintRefs());
}


IRigidBody::~IRigidBody()
{
}


