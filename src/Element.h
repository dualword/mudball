#ifndef ELEMENT_H
#define ELEMENT_H

#include <irrlicht.h>
#include <irrbullet.h>

#include "XEffects/XEffects.h"

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

enum ElementType
{
    EELEVATOR = 1,
    EELEVATOR2X = 2,
    EGOAL = 3,
    EMUD = 4,
    EPLATFORM = 5,
    ERAMP = 6,
    ERAMPEND = 7,
    ERAMPFLAT = 8,
    ERAMPSTART = 9,
    ETUBE2 = 10,
    ETUBE2C = 11,
    ETUBE2H = 12,
    ETUBEBIG = 13,
    ETUBEBIGCURVE = 14,
    ETUBEH = 15,
    EWIND = 16,
};

class Element
{
    public:
        virtual ~Element();

        virtual void animate();
        void setPosition(vector3df pos);
        vector3df getPosition();
        void setRotation(vector3df pos);
        vector3df getRotation();

        ElementType getType();

        void setParent(ISceneNode* parent);

        virtual void setBulletPhysics(irrBulletWorld *world);
        virtual void updateBulletPhysics();

        ISceneNode* getSceneNode();

        virtual void resetElement();

        virtual void addShadow(EffectHandler* effect);

    protected:
        ISceneManager* smgr;
        ISceneNode* node;
        IMesh* mesh;
        ElementType type;

        IGImpactMeshShape* shape;
        IRigidBody* body;

        irrBulletWorld *world;
};

#endif // ELEMENT_H
