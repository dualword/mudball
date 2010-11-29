#ifndef ELEMENT_H
#define ELEMENT_H

#include <irrlicht.h>

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

enum ElementType
{
    EELEVATOR = 1,
    EELEVATOR2X = 2,
    EGOAL = 3,
    EMUD = 4,
    EPLATFORM = 5,
    ERAMP = 6,
    ERAMPEND = 7,
    ERAMPFLAT = 8,
    ERAMPSTART = 9,
    ETUBE2 = 10,
    ETUBE2C = 11,
    ETUBE2H = 12,
    ETUBEBIG = 13,
    ETUBEBIGCURVE = 14,
    ETUBEH = 15,
    EWIND = 16,
    ESTART = 17,
};

class Element
{
    public:
        virtual ~Element();

        virtual void animate();
        void setPosition(vector3df pos);
        vector3df getPosition();
        void setRotation(vector3df pos);
        vector3df getRotation();

        void setParent(ISceneNode* parent);

        ElementType getType();

    protected:
        ISceneManager* smgr;
        ISceneNode* node;
        IMesh* mesh;
        ElementType type;
};

#endif // ELEMENT_H
