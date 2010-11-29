#ifndef ERampEnd_H
#define ERampEnd_H


#include "Element.h"

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class ERampEnd : public Element
{
    public:
        ERampEnd(ISceneManager* smgr);
        //virtual ~ERampEnd();
        virtual void setBulletPhysics(irrBulletWorld *world);

        virtual void updateBulletPhysics();

    protected:
};

#endif // ERampEnd_H
