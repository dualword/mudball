#ifndef ERamp_H
#define ERamp_H


#include "Element.h"

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class ERamp : public Element
{
    public:
        ERamp(ISceneManager* smgr);
        //virtual ~ERamp();
        virtual void setBulletPhysics(irrBulletWorld *world);

        virtual void updateBulletPhysics();

    protected:
};

#endif // ERamp_H
