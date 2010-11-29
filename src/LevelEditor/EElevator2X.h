#ifndef EElevator2X_H
#define EElevator2X_H

#include "Element.h"

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class EElevator2X : public Element
{
    public:
        EElevator2X(ISceneManager* smgr);

        virtual void animate();

    protected:
    private:
        IMesh* platformMesh;
        ISceneNode* platformNode;

        enum Direction
        {
            UP, DOWN
        };

        Direction dir;
        int count;
};

#endif // EElevator2X_H
