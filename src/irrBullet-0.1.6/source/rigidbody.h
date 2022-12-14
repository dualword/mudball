#ifndef __RIGID_BODY_H_INCLUDED__
#define __RIGID_BODY_H_INCLUDED__

#include "common.h"
#include "collisionobject.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"


struct SRigidBodyConstructionInfo
{
    irr::core::vector3df startVector;
    ICollisionShape *collisionShape;
    irr::f32 angularDamping;
    irr::f32 linearDamping;
};


/*!
    The transform space tells the rigid body how to apply the force/torque it receives.
    If the transform space parameter of the function is ERBTS_LOCAL, it will
    apply the force locally with the vector supplied, and vice-versa.

    For example, if you called rigidBody->applyForce(vector3df(0.0,0.0,100.0), ERBTS_LOCAL),
    it would apply a force on the rigid body of 100 in the direction the rigid body is pointing.
    If it was set to ERBTS_WORLD, it would apply a force of 100 in the direction of
    the fixed Z axis of the world. All of the functions default this parameter to ERBTS_WORLD.
*/
enum ERBTransformSpace
{
    ERBTS_LOCAL, // RigidBody forces will be applied in local space
    ERBTS_WORLD  // RigidBody forces will be applied in world space
};


/// The rigid body is the main type for all "hard" simulation objects (The opposite of a soft body).
class IRigidBody : public ICollisionObject
{
public:
    /// @param collShape The collision shape for this body to use.
    IRigidBody(irrBulletWorld* const world, ICollisionShape *collShape);

    IRigidBody(irrBulletWorld* const world, SRigidBodyConstructionInfo info);
    virtual ~IRigidBody();

    void translate(const irr::core::vector3df &v);

    void updateDeactivation(irr::f32 timeStep);

    /// @return If this object is about to be deactivated due to a certain time of inactivity.
    bool wantsSleeping();

    void getAabb(irr::core::vector3df &aabbMin, irr::core::vector3df &aabbMax) const;

    /*!
        Linear velocity is motion in any direction.
        The velocity set here will remain the same throughout the object's lifetime
        unless changed by other means.

        @param transformSpace If this parameter is set to ERBTS_LOCAL, it will only apply
            the velocity in the local space at the time the function was called.
            The velocity will not be applied in current local directions of the rigid body.
            If you wish to achieve this, please see  applyCentralForce(), applyForce(),
            applyCentralImpulse(), applyImpulse(), and internalApplyImpulse().

            However, calling setLinearVelocity() in local space would be great for something
            such as an unguided rocket that only needs one direction at the time of its creation.
    */
    void setLinearVelocity(const irr::core::vector3df &linVel, ERBTransformSpace transformSpace=ERBTS_WORLD);

    /*!
        Angular velocity is the speed at which an object rotates.
        The velocity set here will remain the same throughout the object's lifetime
        unless changed by other means.

        @param transformSpace If this parameter is set to ERBTS_LOCAL, it will only apply
            the velocity in the local space at the time the function was called.
            The velocity will not be applied in current local directions of the rigid body.
            If you wish to achieve this, please see applyTorque(), applyTorqueImpulse() and internalApplyImpulse().
    */
    void setAngularVelocity(const irr::core::vector3df &angVel, ERBTransformSpace transformSpace=ERBTS_WORLD);

    void saveKinematicState(irr::f32 step);

    /// Applies a force from the center of the object.
    void applyCentralForce(const irr::core::vector3df &force, ERBTransformSpace transformSpace=ERBTS_WORLD);

    /*!
        Applies a force from a relative position of the object. This can be useful for things
        such as the thrust from several aircraft engines, rockets, etc.
    */
    void applyForce(const irr::core::vector3df &force, const irr::core::vector3df &relPos, ERBTransformSpace transformSpace=ERBTS_WORLD);

    /// This can be thought of as one quick application of force to induce linear motion.
    void applyCentralImpulse(const irr::core::vector3df &impulse, ERBTransformSpace transformSpace=ERBTS_WORLD);

    /*!
        This is the same as applyCentralImpulse, with the exception that it applies the force
        from a relative position of the object.
    */
    void applyImpulse(const irr::core::vector3df &impulse, const irr::core::vector3df &relPos, ERBTransformSpace transformSpace=ERBTS_WORLD);

    void applyTorque(const irr::core::vector3df &torque, ERBTransformSpace transformSpace=ERBTS_WORLD);

    /// This can be thought of as one quick application of force to induce angular motion (torque).
    void applyTorqueImpulse(const irr::core::vector3df &torque, ERBTransformSpace transformSpace=ERBTS_WORLD);

    /*!
        This simply combines applyImpulse and applyTorqueImpulse into one function.
        Impulses are useful for things like projectiles or forces that may or may not
        need to be continually applied.
        @param linTransformSpace The space to apply the linear impulse.
        @param angTransformSpace The space to apply the angular impulse.
    */
    void internalApplyImpulse(const irr::core::vector3df &linearComponent,
        const irr::core::vector3df &angularComponent, irr::f32 impulseMagnitude,
        ERBTransformSpace linTransformSpace=ERBTS_WORLD, ERBTransformSpace angTransformSpace=ERBTS_WORLD);

    void clearForces();

    void updateInertiaTensor();

    void setDamping(const irr::f32 lin_damping, const irr::f32 ang_damping);

    void applyGravity();

    void setGravity(const irr::core::vector3df &grav);

    void applyDamping(irr::f32 timeStep);

    void setMassProps(irr::f32 mass, const irr::core::vector3df &inertia);

    void setLinearFactor(const irr::core::vector3df &linearFactor);

    void setInvInertiaDiagLocal(const irr::core::vector3df &diagInvInertia);

    void setSleepingThresholds(irr::f32 linear, irr::f32 angular);

    void setAngularFactor(const irr::core::vector3df &angFac);

    /// Sets the new world transform of this object. (rotation and position)
    void setWorldTransform(irr::core::matrix4 irrmat);

    void setAngularFactor(irr::f32 angFac);



    /// @return A pointer to the underlying btRigidBody.
    inline btRigidBody *getPointer() const { return static_cast<btRigidBody*>(object); };

    /*!
        In irrBullet, irr::core::matrix4 takes the place of Bullet's btTransform.
        getWorldTransform() will return a btTransform in Bullet, but returns a matrix4 in irrBullet.
        This matrix includes rotation and position.
    */
    inline irr::core::matrix4 getWorldTransform()
    {
        btTransformToIrrlichtMatrix(getPointer()->getWorldTransform(), worldTransform);
        return worldTransform;
    }

    irr::core::vector3df getLinearVelocity() const;

    irr::core::vector3df getAngularVelocity() const;

    irr::core::vector3df getVelocityInLocalPoint(const irr::core::vector3df &relPos) const;

    irr::f32 computeImpulseDenominator(const irr::core::vector3df &pos, const irr::core::vector3df &normal) const;

    irr::f32 computeAngularImpulseDenominator(const irr::core::vector3df &axis) const;

    irr::core::vector3df getGravity() const;

    irr::f32 getLinearDamping() const;

    irr::f32 getAngularDamping() const;

    irr::f32 getLinearSleepingThreshold() const;

    irr::f32 getAngularSleepingThreshold() const;

    irr::core::vector3df getLinearFactor() const;

    irr::f32 getInvMass() const;

    void integrateVelocities(irr::f32 step);

    irr::core::vector3df getTotalForce() const;

    irr::core::vector3df getTotalTorque();

    irr::core::vector3df getInvInertiaDiagLocal() const;

    irr::core::vector3df getAngularFactor() const;

    /// @return Whether or not the rigid body is in the dynamics world being simulated.
    bool IRigidBody::isInWorld() const;

    bool checkCollideWithOverride(ICollisionObject *co);

    /*! @return The number of constraints that are being referenced by this object.
        Constraints include raycast vehicles, springs, etc.
    */
    irr::u32 getNumConstraintRefs() const;

protected:
    irr::core::matrix4 worldTransform;
};




#endif // __RIGID_BODY_H_INCLUDED__
