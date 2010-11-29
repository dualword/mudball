#ifndef EStart_H
#define EStart_H


#include "Element.h"

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class EStart : public Element
{
    public:
        EStart(ISceneManager* smgr);

        virtual void setBulletPhysics(irrBulletWorld *world);

        virtual void updateBulletPhysics();
        //virtual ~EStart();
    protected:
    private:
};

#endif // EStart_H
