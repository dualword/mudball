#ifndef EELEVATOR_H
#define EELEVATOR_H

#include "Element.h"

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class EElevator : public Element
{
    public:
        EElevator(ISceneManager* smgr);

        virtual void setBulletPhysics(irrBulletWorld *world);
        virtual void animate();

        virtual void updateBulletPhysics();

        virtual void resetElement();

        virtual void addShadow(EffectHandler* effect);

    protected:
    private:
        IMesh* platformMesh;
        ISceneNode* platformNode;

        enum Direction
        {
            UP, DOWN
        };

        Direction dir;
        int count;
};

#endif // EELEVATOR_H
