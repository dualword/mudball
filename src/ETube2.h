#ifndef ETube2_H
#define ETube2_H


#include "Element.h"

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class ETube2 : public Element
{
    public:
        ETube2(ISceneManager* smgr);
        //virtual ~ETube2();
        virtual void setBulletPhysics(irrBulletWorld *world);

        virtual void updateBulletPhysics();

    protected:
};

#endif // ETube2_H
