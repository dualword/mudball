#include <iostream>
#include <irrlicht.h>
#include <vector>

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

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

#include "App.h"

int main()
{
    App::getInstance()->run();
    return 0;
}


int mains()
{

    IrrlichtDevice* device = createDevice( video::EDT_OPENGL, dimension2d<u32>(640, 480), 16,false, false, true, 0);

    //EventReceiver* recv = new EventReceiver(device);
    //device->setEventReceiver(recv);

    IVideoDriver* driver = device->getVideoDriver();
    ISceneManager* smgr = device->getSceneManager();
    IGUIEnvironment* guienv = device->getGUIEnvironment();

    smgr->addCameraSceneNodeFPS(0,100,0.01);
    smgr->setAmbientLight(SColorf(1,1,1,1));

    vector<Element*> el;

    Element* e = new EElevator2X(smgr);
    el.push_back(e);

    Element* plat = new EPlatform(smgr);
    plat->setPosition(vector3df(0,0,-2));
    el.push_back(plat);

    Element* mud = new EMud(smgr);
    mud->setPosition(vector3df(-2,0,-2));
    el.push_back(mud);

    Element* goal = new EGoal(smgr);
    goal->setPosition(vector3df(-4,0,-2));
    el.push_back(goal);

    Element* tubeBig = new ETubeBig(smgr);
    tubeBig->setPosition(vector3df(-6,0,-2));
    el.push_back(tubeBig);

    Element* tubeBigC = new ETubeBigCurve(smgr);
    tubeBigC->setPosition(vector3df(-6,0,0));
    el.push_back(tubeBigC);

    Element* wind = new EWind(smgr);
    wind->setPosition(vector3df(-6,0,-4));
    el.push_back(wind);

    Element* plat2 = new EPlatform(smgr);
    plat2->setPosition(vector3df(-6,0,-6));
    el.push_back(plat2);

    Element* rampFlat = new ERampFlat(smgr);
    rampFlat->setPosition(vector3df(4,1,0));
    rampFlat->setRotation(vector3df(0,-90,0));
    el.push_back(rampFlat);

    Element* platRamp = new EPlatform(smgr);
    platRamp->setPosition(vector3df(2,2,0));
    el.push_back(platRamp);

    Element* pRampStart = new ERampStart(smgr);
    pRampStart->setPosition(vector3df(2,1,-2));
    el.push_back(pRampStart);

    Element* pRamp = new ERamp(smgr);
    pRamp->setPosition(vector3df(2,0,-4));
    el.push_back(pRamp);

    Element* pRampEnd = new ERampEnd(smgr);
    pRampEnd->setPosition(vector3df(2,0,-6));
    el.push_back(pRampEnd);

    Element* pEle= new EElevator(smgr);
    pEle->setPosition(vector3df(2,0,-6));
    el.push_back(pEle);

    Element* pTubeH= new ETubeH(smgr);
    pTubeH->setPosition(vector3df(8,0,-6));
    el.push_back(pTubeH);

    Element* pTube2H= new ETube2H(smgr);
    pTube2H->setPosition(vector3df(6,0,-6));
    el.push_back(pTube2H);

    Element* pTube2= new ETube2(smgr);
    pTube2->setPosition(vector3df(4,0,-6));
    el.push_back(pTube2);

    Element* pTube2C= new ETube2C(smgr);
    pTube2C->setPosition(vector3df(4,0,-8));
    el.push_back(pTube2C);

    while(device->run())
    {
        for(int i=0;i<el.size();i++) el[i]->animate();

        driver->beginScene(true, true, SColor(255,100,101,140));

        smgr->drawAll();
        guienv->drawAll();

        driver->endScene();
    }

    return 0;
}

