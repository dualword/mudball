//MudBall-mod https://github.com/dualword/mudball
#include "Bullet/btBulletDynamicsCommon.h"
#include "Bullet/btBulletCollisionCommon.h"
#include "motionstate.h"
#include "bulletworld.h"

using namespace irr;
using namespace core;
using namespace scene;

IMotionState::IMotionState(const btTransform &initialPos)
{
    worldTransform = initialPos;
}

void IMotionState::getWorldTransform(btTransform &worldTrans) const
{
    /*if(object->hasParent == true)
    {
        btTransform tempTrans = object->body->getWorldTransform();

        btVector3 pos = object->getParent()->body->getWorldTransform().getOrigin() - object->body->getWorldTransform().getOrigin();

        tempTrans.setOrigin(pos);

        worldTrans = tempTrans;
    }*/



    worldTrans = worldTransform;
}

void IMotionState::setWorldTransform(const btTransform &worldTrans)
{
    if(object)
    {
        /*if(object->hasParent == true)
        {
            btTransform tempTrans = worldTrans;

            btVector3 pos = object->getParent()->body->getWorldTransform().getOrigin() + object->body->getWorldTransform().getOrigin();

            tempTrans.setOrigin(pos);

            applyTransformToVisualModel(tempTrans);

            worldTransform = tempTrans;

            //printf(object->worldObjectName.c_str());
        }*/

        ISceneNode *node = object->getCollisionShape()->getSceneNode();

        irr::core::matrix4 matr;
        btTransformToIrrlichtMatrix(worldTrans, matr);

        node->setRotation(matr.getRotationDegrees());
        node->setPosition(matr.getTranslation());

        worldTransform = worldTrans;
    }

    else
    {
        if(failed == false)
        {
            wchar_t *str = 0;

//            wsprintfW( str, L"Object ( %s ) could not be updated.\n"),
//                object->getCollisionShape()->getSceneNode()->getName();
//            MessageBoxW(0, str, L"irrBullet Error", MB_OK | MB_ICONERROR );
            failed = true;
        }

    }
}


IMotionState::~IMotionState()
{
}

