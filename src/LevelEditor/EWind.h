#ifndef EWind_H
#define EWind_H


#include "Element.h"

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class EWind : public Element
{
    public:
        EWind(ISceneManager* smgr);
        //virtual ~EWind();

        virtual void animate();

    protected:
    private:
        ISceneNode* hNode;
        ISceneNode* wNode;
};

#endif // EWind_H
