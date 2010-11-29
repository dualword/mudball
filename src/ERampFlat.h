#ifndef ERampFlat_H
#define ERampFlat_H


#include "Element.h"

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class ERampFlat : public Element
{
    public:
        ERampFlat(ISceneManager* smgr);
        //virtual ~ERampFlat();
        virtual void setBulletPhysics(irrBulletWorld *world);

        virtual void updateBulletPhysics();

    protected:
};

#endif // ERampFlat_H
