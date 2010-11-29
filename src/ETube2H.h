#ifndef ETube2H_H
#define ETube2H_H


#include "Element.h"

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class ETube2H : public Element
{
    public:
        ETube2H(ISceneManager* smgr);
        //virtual ~ETube2H();
        virtual void setBulletPhysics(irrBulletWorld *world);

        virtual void updateBulletPhysics();

    protected:
};

#endif // ETube2H_H
