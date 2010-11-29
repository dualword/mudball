#ifndef ERampStart_H
#define ERampStart_H


#include "Element.h"

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class ERampStart : public Element
{
    public:
        ERampStart(ISceneManager* smgr);
        //virtual ~ERampStart();
        virtual void setBulletPhysics(irrBulletWorld *world);

        virtual void updateBulletPhysics();

    protected:
};

#endif // ERampStart_H
