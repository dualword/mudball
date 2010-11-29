#ifndef ETubeBigCurve_H
#define ETubeBigCurve_H


#include "Element.h"

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class ETubeBigCurve : public Element
{
    public:
        ETubeBigCurve(ISceneManager* smgr);
        //virtual ~ETubeBigCurve();
        virtual void setBulletPhysics(irrBulletWorld *world);

        virtual void updateBulletPhysics();

    protected:
};

#endif // ETubeBigCurve_H
