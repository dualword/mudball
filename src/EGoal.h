#ifndef EGoal_H
#define EGoal_H


#include "Element.h"

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class EGoal : public Element
{
    public:
        EGoal(ISceneManager* smgr);

        virtual void animate();
        virtual void setBulletPhysics(irrBulletWorld *world);

        virtual void updateBulletPhysics();

    protected:
    private:
        ISceneNode* blueNode;
};

#endif // EGoal_H
