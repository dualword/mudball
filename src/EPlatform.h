#ifndef EPLATFORM_H
#define EPLATFORM_H


#include "Element.h"

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class EPlatform : public Element
{
    public:
        EPlatform(ISceneManager* smgr);
        virtual void setBulletPhysics(irrBulletWorld *world);

        virtual void updateBulletPhysics();

        //virtual ~EPlatform();
    protected:
    private:
};

#endif // EPLATFORM_H
