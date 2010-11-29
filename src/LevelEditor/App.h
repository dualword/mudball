#ifndef APP_H
#define APP_H

#include "EElevator.h"
#include "EElevator2X.h"
#include "EPlatform.h"
#include "EMud.h"
#include "EGoal.h"
#include "ETubeBig.h"
#include "ETubeBigCurve.h"
#include "ETubeH.h"
#include "ETube2.h"
#include "ETube2C.h"
#include "ETube2H.h"
#include "EWind.h"
#include "ERampFlat.h"
#include "ERamp.h"
#include "ERampStart.h"
#include "ERampEnd.h"
#include "EStart.h"

#include <vector>

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

using namespace std;


class App
{
    public:
        static App* getInstance();

        void eventKeyPressed(int key);
        void eventKeyReleased(int key);

        void run();

        virtual ~App();
    protected:
    private:
        App();

        IrrlichtDevice* device;
        IVideoDriver* driver;
        ISceneManager* smgr;
        IGUIEnvironment* guienv;

        ICameraSceneNode* camera;
        IEventReceiver* recv;

        ISceneNode* wcube;

        vector<Element*> level;

        int currentElement;

        Element* existElementAt(vector3df pos);
        Element* addElementAt(vector3df pos);

        void removeElementAt(vector3df pos);

        void saveToXML();
        void loadFromXML();
};

#endif // APP_H
