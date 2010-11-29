#ifndef ETubeBig_H
#define ETubeBig_H


#include "Element.h"

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class ETubeBig : public Element
{
    public:
        ETubeBig(ISceneManager* smgr);
        //virtual ~ETubeBig();
        virtual void setBulletPhysics(irrBulletWorld *world);

        virtual void updateBulletPhysics();

    protected:
};

#endif // ETubeBig_H
